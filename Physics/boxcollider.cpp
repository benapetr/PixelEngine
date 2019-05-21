// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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

    return this->PositionMatch(collider->Position);
}


