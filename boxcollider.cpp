//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "boxcollider.h"
#include "bitmapcollider.h"
#include "collidermath.h"

using namespace PE;

BoxCollider::BoxCollider(double x, double y, double w, double h)
{
    this->Position.X = x;
    this->Position.Y = y;
    this->RelativePosition = this->Position;
    this->width = w;
    this->height = h;
}

bool BoxCollider::PositionMatch(Vector position)
{
    if (position.X < this->Position.X)
        return false;
    if (position.Y < this->Position.Y)
        return false;
    if (position.X > this->Position.X + this->width)
        return false;
    if (position.Y > this->Position.Y + this->height)
        return false;
    return true;
}

bool BoxCollider::IntersectionMatch(Collider *collider)
{
    if (collider->GetColliderType() == PE::PE_ColliderType_Box)
        return ColliderMath::IntersectionCheckBoxBox(this, dynamic_cast<BoxCollider*>(collider));

    if (collider->GetColliderType() == PE::PE_ColliderType_Bitmap)
        return ColliderMath::IntersectionCheckBoxBitmap(this, dynamic_cast<BitmapCollider*>(collider));

    return false;
}


