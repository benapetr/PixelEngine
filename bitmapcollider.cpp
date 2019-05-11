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

using namespace PE;

BitmapCollider::BitmapCollider(int x, int y, int w, int h)
{
    this->width = w;
    this->height = h;
}

bool BitmapCollider::PositionMatch(Vector position)
{
    return false;
}

bool BitmapCollider::IntersectionMatch(Collider *collider)
{
    return false;
}
