//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "waterdrop.h"
#include <PixelEngine/camera.h>
#include <PixelEngine/Physics/rigidbody.h>
#include <PixelEngine/Physics/pixelcollider.h>
#include <PixelEngine/Graphics/renderer.h>

WaterDrop::WaterDrop(const PE::Vector& position) : PE::Actor(position)
{
    this->RigidBody = new PE::Rigidbody();
    this->AddChildren(new PE::PixelCollider(0, 0));
    this->Destroy(2000);
}

void WaterDrop::Render(PE::Renderer *r, PE::Camera *c)
{
    PE::Vector position = c->ProjectedPosition(this->Position);
    r->DrawEllipse(position.X2int() - 1, position.Y2int() - 1, 2, 2, Qt::darkBlue, 4);
}

void WaterDrop::Event_OnImpact(const PE::Vector &impact_force)
{
    this->Destroy();
}
