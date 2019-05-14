//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "bitmapcollider.h"
#include "collidermath.h"
#include "boxcollider.h"
#include "exception.h"

using namespace PE;

BitmapCollider::BitmapCollider(int x, int y, int w, int h)
{
    if (w > PE_BITSET_MAX)
        throw new Exception("Width of BitMap collider can't be higher than PE_BITSET_MAX", BOOST_CURRENT_FUNCTION);

    this->Position = Vector(x, y);
    this->RelativePosition = Vector(x, y);
    this->width = w;
    this->height = h;
    int r = w;
    while (r-- > 0)
        this->Bitmap.append(pe_bitset());
}

bool BitmapCollider::PositionMatch(Vector position)
{
    if (position.X < this->Position.X || position.X > this->Position.X + static_cast<double>(this->width))
        return false;
    if (position.Y < this->Position.Y || position.Y > this->Position.Y + static_cast<double>(this->height))
        return false;

    return (this->Bitmap[position.X2int()][position.Y2int()]);
}

bool BitmapCollider::IntersectionMatch(Collider *collider)
{
    if (collider->GetColliderType() == PE_ColliderType_Box)
        return ColliderMath::IntersectionCheckBoxBitmap(dynamic_cast<BoxCollider*>(collider), this);

    return false;
}
