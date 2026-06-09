//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include <cmath>
#include "collidermath.h"
#include "circlecollider.h"
#include "boxcollider.h"
#include "bitmapcollider.h"

using namespace PE;

namespace
{
    struct Projection
    {
        pe_float_t Min;
        pe_float_t Max;
    };

    bool aabbOverlap(pe_float_t ax, pe_float_t ay, pe_float_t aw, pe_float_t ah,
                     pe_float_t bx, pe_float_t by, pe_float_t bw, pe_float_t bh)
    {
        return ax < bx + bw &&
               ax + aw > bx &&
               ay < by + bh &&
               ay + ah > by;
    }

    void boxWorldAabb(BoxCollider *box, pe_float_t *x, pe_float_t *y, pe_float_t *w, pe_float_t *h)
    {
        Vector corners[] = { box->A(), box->B(), box->C(), box->D() };
        pe_float_t min_x = corners[0].X;
        pe_float_t max_x = corners[0].X;
        pe_float_t min_y = corners[0].Y;
        pe_float_t max_y = corners[0].Y;

        for (int i = 1; i < 4; i++)
        {
            min_x = std::min(min_x, corners[i].X);
            max_x = std::max(max_x, corners[i].X);
            min_y = std::min(min_y, corners[i].Y);
            max_y = std::max(max_y, corners[i].Y);
        }

        *x = min_x;
        *y = min_y;
        *w = max_x - min_x;
        *h = max_y - min_y;
    }

    bool boxBitmapAabbOverlap(BoxCollider *box, BitmapCollider *bitmap)
    {
        pe_float_t x;
        pe_float_t y;
        pe_float_t w;
        pe_float_t h;
        boxWorldAabb(box, &x, &y, &w, &h);
        return aabbOverlap(x, y, w, h, bitmap->Position.X, bitmap->Position.Y, bitmap->GetWidth(), bitmap->GetHeight());
    }

    bool circleBitmapAabbOverlap(CircleCollider *circle, BitmapCollider *bitmap)
    {
        pe_float_t radius = circle->Radius * circle->Scale;
        return aabbOverlap(circle->Position.X - radius, circle->Position.Y - radius, radius * 2, radius * 2,
                           bitmap->Position.X, bitmap->Position.Y, bitmap->GetWidth(), bitmap->GetHeight());
    }

    pe_float_t sampleStep(pe_float_t span)
    {
        return std::max(static_cast<pe_float_t>(1), std::min(static_cast<pe_float_t>(4), span / 8));
    }

    Vector edgeAxis(Vector a, Vector b)
    {
        Vector edge(b.X - a.X, b.Y - a.Y);
        Vector axis(-edge.Y, edge.X);
        pe_float_t length = std::sqrt((axis.X * axis.X) + (axis.Y * axis.Y));
        if (length <= static_cast<pe_float_t>(0.0001))
            return Vector(0, 0);

        return Vector(axis.X / length, axis.Y / length);
    }

    Projection projectCorners(const Vector *corners, Vector axis)
    {
        pe_float_t dot = (corners[0].X * axis.X) + (corners[0].Y * axis.Y);
        Projection projection = { dot, dot };

        for (int i = 1; i < 4; i++)
        {
            dot = (corners[i].X * axis.X) + (corners[i].Y * axis.Y);
            projection.Min = std::min(projection.Min, dot);
            projection.Max = std::max(projection.Max, dot);
        }

        return projection;
    }

    bool projectionsOverlap(Projection a, Projection b)
    {
        return a.Max >= b.Min && b.Max >= a.Min;
    }

    bool boxSatOverlap(BoxCollider *a, BoxCollider *b)
    {
        Vector ac[] = { a->A(), a->B(), a->C(), a->D() };
        Vector bc[] = { b->A(), b->B(), b->C(), b->D() };
        Vector axes[] = {
            edgeAxis(ac[0], ac[1]),
            edgeAxis(ac[1], ac[2]),
            edgeAxis(bc[0], bc[1]),
            edgeAxis(bc[1], bc[2])
        };

        for (const Vector &axis : axes)
        {
            if (axis.X == 0 && axis.Y == 0)
                continue;

            if (!projectionsOverlap(projectCorners(ac, axis), projectCorners(bc, axis)))
                return false;
        }

        return true;
    }

    bool boxCircleOverlap(BoxCollider *box, CircleCollider *circle)
    {
        Vector local_circle = box->WorldToLocal(circle->Position);
        pe_float_t left = box->Position.X;
        pe_float_t right = box->Position.X + (box->Width * box->Scale);
        pe_float_t bottom = box->Position.Y;
        pe_float_t top = box->Position.Y + (box->Height * box->Scale);
        pe_float_t closest_x = std::max(left, std::min(local_circle.X, right));
        pe_float_t closest_y = std::max(bottom, std::min(local_circle.Y, top));
        pe_float_t dx = local_circle.X - closest_x;
        pe_float_t dy = local_circle.Y - closest_y;
        pe_float_t radius = circle->Radius * circle->Scale;

        return (dx * dx) + (dy * dy) <= radius * radius;
    }

    bool sampleLineAgainstBitmap(Vector a, Vector b, BitmapCollider *bitmap)
    {
        pe_float_t dx = b.X - a.X;
        pe_float_t dy = b.Y - a.Y;
        pe_float_t length = std::sqrt((dx * dx) + (dy * dy));
        pe_float_t step = sampleStep(length);
        pe_float_t samples = std::max(static_cast<pe_float_t>(1), std::ceil(length / step));

        for (pe_float_t i = 0; i <= samples; i++)
        {
            pe_float_t t = i / samples;
            if (bitmap->PositionMatch(Vector(a.X + (dx * t), a.Y + (dy * t))))
                return true;
        }

        return false;
    }
}

bool ColliderMath::IntersectionCheckLineCircle(Vector a, Vector b, CircleCollider *c)
{
    // Doesn't work:
    /*

    Vector d;
    // https://stackoverflow.com/questions/23772990/find-point-with-vector-projection
    double CF = ((b.X-a.X) * (c->Position.X-a.X)+(b.Y-a.Y) * (c->Position.Y-a.Y)) / (std::pow(b.X-a.X, 2) + std::pow(b.Y-a.Y, 2));
    d.X = a.X + ((b.X-a.X) * CF);
    d.Y = a.Y + ((b.Y-a.Y) * CF);

    // Now just check if this point is inside
    return c->PositionMatch(d);
    */

    //https://stackoverflow.com/a/1084899/1514983
    pe_float_t radius = c->Radius * c->Scale;

    Vector direction = b - a;
    Vector sphere_centre_to_ray = a - c->Position;

    pe_float_t dot_a = direction.Dot();
    pe_float_t dot_b = 2 * sphere_centre_to_ray.Dot(direction);
    pe_float_t dot_c = sphere_centre_to_ray.Dot() - (radius * radius);
    pe_float_t discriminant = (dot_b * dot_b) - (4 * dot_a * dot_c);
    if( discriminant < 0 )
    {
        // no intersection
        return false;
    }
    else
    {
        // ray didn't totally miss sphere,
        // so there is a solution to
        // the equation.
        discriminant = std::sqrt(discriminant);
        // either solution may be on or off the ray so need to test both
        // t1 is always the smaller value, because BOTH discriminant and
        // a are nonnegative.
        pe_float_t t1 = (-dot_b - discriminant)/(2*dot_a);
        pe_float_t t2 = (-dot_b + discriminant)/(2*dot_a);

        // 3x HIT cases:
        //          -o->             --|-->  |            |  --|->
        // Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit),

        // 3x MISS cases:
        //       ->  o                     o ->              | -> |
        // FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

        if( t1 >= 0 && t1 <= 1 )
        {
            // t1 is the intersection, and it's closer than t2
            // (since t1 uses -b - discriminant)
            // Impale, Poke
            return true;
        }

        // here t1 didn't intersect so we are either started
        // inside the sphere or completely past it
        if( t2 >= 0 && t2 <= 1 )
        {
            // ExitWound
            return true;
        }

        // no intn: FallShort, Past, CompletelyInside
        return false;
    }
}

bool ColliderMath::IntersectionCheckBoxBox(BoxCollider *a, BoxCollider *b)
{
    if (a->IsOriented() || b->IsOriented())
        return boxSatOverlap(a, b);

    if (a->Position.X < b->Position.X + (b->Width * b->Scale) &&
        a->Position.X + (a->Width * a->Scale) > b->Position.X &&
        a->Position.Y < b->Position.Y + (b->Height * b->Scale) &&
        a->Position.Y + (a->Height * a->Scale) > b->Position.Y)
    {
        // there is collision
        return true;
    }

    return false;
}

bool ColliderMath::IntersectionCheckBoxBitmap(BoxCollider *a, BitmapCollider *b)
{
    if (!boxBitmapAabbOverlap(a, b))
        return false;

    if (a->IsOriented())
    {
        return sampleLineAgainstBitmap(a->A(), a->B(), b) ||
               sampleLineAgainstBitmap(a->B(), a->C(), b) ||
               sampleLineAgainstBitmap(a->C(), a->D(), b) ||
               sampleLineAgainstBitmap(a->D(), a->A(), b);
    }

    pe_float_t left = a->Position.X;
    pe_float_t right = a->Position.X + (a->Width * a->Scale);
    pe_float_t bottom = a->Position.Y;
    pe_float_t top = a->Position.Y + (a->Height * a->Scale);
    pe_float_t step_x = sampleStep(right - left);
    pe_float_t step_y = sampleStep(top - bottom);

    for (pe_float_t x = left; x <= right; x += step_x)
    {
        if (b->PositionMatch(Vector(x, bottom)) || b->PositionMatch(Vector(x, top)))
            return true;
    }

    for (pe_float_t y = bottom; y <= top; y += step_y)
    {
        if (b->PositionMatch(Vector(left, y)) || b->PositionMatch(Vector(right, y)))
            return true;
    }

    return false;
}

bool ColliderMath::IntersectionCheckBoxCircle(BoxCollider *a, CircleCollider *b)
{
    return boxCircleOverlap(a, b);
}

bool ColliderMath::IntersectionCheckCircleBitmap(BitmapCollider *a, CircleCollider *b)
{
    if (!circleBitmapAabbOverlap(b, a))
        return false;

    if (a->PositionMatch(b->Position))
        return true;

    pe_float_t radius = b->Scale * b->Radius;
    pe_float_t diagonal_radius = radius * static_cast<pe_float_t>(0.70710678118);
    Vector samples[] = {
        Vector(b->Position.X - radius, b->Position.Y),
        Vector(b->Position.X + radius, b->Position.Y),
        Vector(b->Position.X, b->Position.Y - radius),
        Vector(b->Position.X, b->Position.Y + radius),
        Vector(b->Position.X - diagonal_radius, b->Position.Y - diagonal_radius),
        Vector(b->Position.X + diagonal_radius, b->Position.Y - diagonal_radius),
        Vector(b->Position.X - diagonal_radius, b->Position.Y + diagonal_radius),
        Vector(b->Position.X + diagonal_radius, b->Position.Y + diagonal_radius)
    };

    for (const Vector &sample : samples)
    {
        if (a->PositionMatch(sample))
            return true;
    }

    return false;
}

bool ColliderMath::IntersectionCheckCircleCircle(CircleCollider *a, CircleCollider *b)
{
    return (b->Position.DistanceTo(a->Position) <= (a->Radius * a->Scale) + (b->Radius * b->Scale));
}
