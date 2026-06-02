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
    bool aabbOverlap(pe_float_t ax, pe_float_t ay, pe_float_t aw, pe_float_t ah,
                     pe_float_t bx, pe_float_t by, pe_float_t bw, pe_float_t bh)
    {
        return ax < bx + bw &&
               ax + aw > bx &&
               ay < by + bh &&
               ay + ah > by;
    }

    bool boxBitmapAabbOverlap(BoxCollider *box, BitmapCollider *bitmap)
    {
        return aabbOverlap(box->Position.X, box->Position.Y, box->Width * box->Scale, box->Height * box->Scale,
                           bitmap->Position.X, bitmap->Position.Y, bitmap->GetWidth(), bitmap->GetHeight());
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
    // This doesn't seem to work - taken from https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
    /*
    double cdist_x = std::abs(b->Position.X - a->Position.X);
    double cdist_y = std::abs(b->Position.Y - a->Position.Y);
    if (cdist_x > (a->Width / 2 + b->Radius))
        return false;
    if (cdist_y > (a->Height / 2 + b->Radius))
        return false;
    double cdist = std::pow(cdist_x - a->Width / 2, 2) + std::pow(cdist_y - a->Height / 2, 2);
    return (cdist <= std::pow(b->Radius, 2));
    */


    // Our own idiot-proof implementation - let's check intersection of individual points
    // Circle centre in box
    if (a->PositionMatch(b->Position))
        return true;

    // Line checks
    // AB
    if (IntersectionCheckLineCircle(a->A(), a->B(), b))
        return true;
    // BC
    if (IntersectionCheckLineCircle(a->B(), a->C(), b))
        return true;
    // CD
    if (IntersectionCheckLineCircle(a->C(), a->D(), b))
        return true;
    // DA
    if (IntersectionCheckLineCircle(a->D(), a->A(), b))
        return true;

    return false;
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
