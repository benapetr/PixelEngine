// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef ELLIPTICCOLLIDER_H
#define ELLIPTICCOLLIDER_H

#include "collider.h"

namespace PE
{
    class EllipticCollider : public Collider
    {
        public:
            EllipticCollider();
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Ellipse; }
    };
}

#endif // ELLIPTICCOLLIDER_H
