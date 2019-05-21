// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "bitmapcollider.h"
#include "collidermath.h"
#include "boxcollider.h"
#include "../exception.h"

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
    // This check is probably not necessary, we do more advanced boundary check
    if (position.X < this->Position.X || position.X > this->Position.X + static_cast<double>(this->width))
        return false;
    if (position.Y < 0 || position.Y < this->Position.Y || position.Y > this->Position.Y + static_cast<double>(this->height))
        return false;

    // Shift the position by the collider position vector
    int position_x = position.X2int() - this->Position.X2int();
    int position_y = position.Y2int() - this->Position.Y2int();

    // Check again if this position is within bitmap boundary
    if (position_x < 0 || position_x > this->width)
        return false;
    if (position_y < 0 || position_y > this->height)
        return false;

    return (this->Bitmap[position_x][position_y]);
}

bool BitmapCollider::IntersectionMatch(Collider *collider)
{
    if (collider->GetColliderType() == PE_ColliderType_Box)
        return ColliderMath::IntersectionCheckBoxBitmap(dynamic_cast<BoxCollider*>(collider), this);

    return this->PositionMatch(collider->Position);
}
