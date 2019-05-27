// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H

#include "collider.h"

namespace PE
{
    class CircleCollider : public Collider
    {
        public:
            CircleCollider(double x, double y, double radius);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Circle; }
            double Radius;
    };
}

#endif // CIRCLECOLLIDER_H
