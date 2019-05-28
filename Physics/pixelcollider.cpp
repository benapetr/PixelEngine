//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "pixelcollider.h"

using namespace PE;

PixelCollider::PixelCollider(double x, double y)
{
    this->Position = Vector(x, y);
}

bool PixelCollider::PositionMatch(Vector position)
{
    return position == this->Position;
}

bool PixelCollider::IntersectionMatch(Collider *collider)
{
    return collider->PositionMatch(this->Position);
}

PE_ColliderType PixelCollider::GetColliderType()
{
    return PE_ColliderType_Pixel;
}
