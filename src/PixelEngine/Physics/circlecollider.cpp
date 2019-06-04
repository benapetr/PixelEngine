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
#include "circlecollider.h"
#ifdef PE_DEBUG
#include "../camera.h"
#include "../Graphics/renderer.h"
#endif

using namespace PE;

CircleCollider::CircleCollider(double x, double y, double radius)
{
    this->Radius = radius;
    this->Position = Vector(x, y);
}

bool PE::CircleCollider::PositionMatch(PE::Vector position)
{
    return (this->Position.DistanceTo(position) <= (this->Radius * this->Scale));
}

bool CircleCollider::IntersectionMatch(Collider *collider)
{
    switch(collider->GetColliderType())
    {
        case PE_ColliderType_Pixel:
            return this->PositionMatch(collider->Position);
        case PE_ColliderType_Circle:
            return ColliderMath::IntersectionCheckCircleCircle(dynamic_cast<CircleCollider*>(collider), this);
        case PE_ColliderType_Box:
            return ColliderMath::IntersectionCheckBoxCircle(dynamic_cast<BoxCollider*>(collider), this);
        case PE_ColliderType_Bitmap:
            return ColliderMath::IntersectionCheckCircleBitmap(dynamic_cast<BitmapCollider*>(collider), this);
        case PE_ColliderType_Ellipse:
            // not implemented yet
            return this->PositionMatch(collider->Position);
    }

    return this->PositionMatch(collider->Position);
}

#ifdef PE_DEBUG
void CircleCollider::Render(Renderer *r, Camera *c)
{
    if (!Collider::Debug)
        return;
    PE::Vector root = c->ProjectedPosition(this->Position) - (this->Radius * this->Scale);
    int w = static_cast<int>((this->Radius * 2 * this->Scale));
    r->DrawEllipse(root.X2int(), root.Y2int(), w, w, Qt::green);
}
#endif
