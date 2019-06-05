//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include <PixelEngine/camera.h>
#include <cmath>
#include "radian1.h"
#include <PixelEngine/Graphics/renderer.h>

Radian1::Radian1(const PE::Vector &position)
{
    v1 = position;
}

void Radian1::Update(qint64 time)
{
    v2 = v1;
    //v2.X = v2.X() * std::
}

void Radian1::Render(PE::Renderer *r, PE::Camera *c)
{
    PE::Vector position = c->ProjectedPosition(this->Position);
    //r->DrawLine(this->Position)
}
