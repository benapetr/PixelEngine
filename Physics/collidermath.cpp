// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include <cmath>
#include "collidermath.h"
#include "circlecollider.h"
#include "boxcollider.h"
#include "bitmapcollider.h"

using namespace PE;

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
    double cdist_x = std::abs(b->Position.X - a->Position.X);
    double cdist_y = std::abs(b->Position.Y - a->Position.Y);
    if (cdist_x > (a->Width / 2 + b->Radius))
        return false;
    if (cdist_y > (a->Height / 2 + b->Radius))
        return false;
    double cdist = std::pow(cdist_x - a->Width / 2, 2) + std::pow(cdist_y - a->Height / 2, 2);
    return (cdist <= std::pow(b->Radius, 2));
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
