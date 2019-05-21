// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
