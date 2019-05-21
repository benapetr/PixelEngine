// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef PIXELCOLLIDER_H
#define PIXELCOLLIDER_H

#include "collider.h"

namespace PE
{
    class PixelCollider : public Collider
    {
        public:
            PixelCollider(double x, double y);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            PE_ColliderType GetColliderType() override;
    };
}

#endif // PIXELCOLLIDER_H
