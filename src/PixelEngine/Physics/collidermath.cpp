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

bool ColliderMath::IntersectionCheckLineCircle(Vector a, Vector b, CircleCollider *c)
{
    Vector d;
    // https://stackoverflow.com/questions/23772990/find-point-with-vector-projection
    double CF = ((b.X-a.X)*(c->Position.X-a.X)+(b.Y-a.Y)*(c->Position.Y-a.Y))/(std::pow(b.X-a.X, 2)+std::pow(b.Y-a.Y, 2));
    d.X = a.X+(b.X-a.X) * CF;
    d.Y = a.Y+(b.Y-a.Y) * CF;

    // Now just check if this point is inside
    return c->PositionMatch(d);
}

bool ColliderMath::IntersectionCheckBoxBox(BoxCollider *a, BoxCollider *b)
{
    if (a->Position.X < b->Position.X + b->Width &&
        a->Position.X + a->Width > b->Position.X &&
        a->Position.Y < b->Position.Y + b->Height &&
        a->Position.Y + a->Height > b->Position.Y)
    {
        // there is collision
        return true;
    }

    return false;
}

bool ColliderMath::IntersectionCheckBoxBitmap(BoxCollider *a, BitmapCollider *b)
{
    // Check collision vs corners (4 points)
    if (b->PositionMatch(a->Position))
        return true;

    Vector p = a->Position;
    p.X += a->Width / 2;
    if (b->PositionMatch(p))
        return true;
    p.X += a->Width;
    if (b->PositionMatch(p))
        return true;

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
    if (a->PositionMatch(b->Position))
        return true;
    // Calculate 4 points of circle and check each of them
    Vector v1(b->Position), v2(b->Position), v3(b->Position), v4(b->Position);

    v1.X -= b->Radius;
    v2.X += b->Radius;
    v3.Y -= b->Radius;
    v3.Y += b->Radius;

    if (a->PositionMatch(v1))
        return true;
    if (a->PositionMatch(v2))
        return true;
    if (a->PositionMatch(v3))
        return true;
    if (a->PositionMatch(v4))
        return true;

    return false;
}

bool ColliderMath::IntersectionCheckCircleCircle(CircleCollider *a, CircleCollider *b)
{
    return (b->Position.DistanceTo(a->Position) <= a->Radius + b->Radius);
}
