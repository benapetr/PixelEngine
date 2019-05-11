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

using namespace PE;

BoxCollider::BoxCollider(double x, double y, double w, double h)
{
    this->X = x;
    this->Y = y;
    this->width = w;
    this->height = h;
}

bool BoxCollider::PositionMatch(Vector position)
{
    if (position.X < this->X)
        return false;
    if (position.Y < this->Y)
        return false;
    if (position.X > this->X + this->width)
        return false;
    if (position.Y > this->Y + this->height)
        return false;
    return true;
}

