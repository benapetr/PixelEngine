//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "collidermath.h"
#include "boxcollider.h"
#include "bitmapcollider.h"

using namespace PE;

bool ColliderMath::IntersectionCheckBoxBox(BoxCollider *a, BoxCollider *b)
{
    if (a->Position.X < b->Position.X + b->width &&
        a->Position.X + a->width > b->Position.X &&
        a->Position.Y < b->Position.Y + b->height &&
        a->Position.Y + a->height > b->Position.Y)
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
    p.X += a->width / 2;
    if (b->PositionMatch(p))
        return true;
    p.X += a->width;
    if (b->PositionMatch(p))
        return true;

    return false;
}
