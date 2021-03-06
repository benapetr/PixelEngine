//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "r1.h"
#include <PixelEngine/camera.h>
#include <PixelEngine/world.h>
#include <PixelEngine/pemath.h>
#include <PixelEngine/Physics/boxcollider.h>
#include <PixelEngine/Physics/rigidbody.h>
#include <PixelEngine/Graphics/renderer.h>

R1::R1(const PE::Vector &position) : PE::Actor(position)
{
    this->RigidBody = new PE::Rigidbody();
    this->AddChildren(new PE::BoxCollider(0, 0, 20, 10));
}

void R1::Render(PE::Renderer *r, PE::Camera *c)
{
    PE::Vector position = c->ProjectedPosition(this->Position);
    r->DrawRect(position.X2int(), position.Y2int(), 20, 10, 2, Qt::blue);
}
