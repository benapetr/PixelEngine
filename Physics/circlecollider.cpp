// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
    return (this->Position.DistanceTo(position) <= this->Radius);
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
    PE::Vector root = c->ProjectedPosition(this->Position) - this->Radius;
    int w = static_cast<int>(this->Radius * 2);
    r->DrawEllipse(root.X2int(), root.Y2int(), w, w, Qt::green);
}
#endif
