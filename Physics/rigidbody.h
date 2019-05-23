// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../vector.h"

namespace PE
{
    class Collider;
    class Rigidbody
    {
        public:
            Rigidbody();
            // If object is not grounded, gravity will apply to it, as long as its Weight is higher than 0
            bool IsGrounded();
            Vector GetMovement();
            void AddForce(const Vector &force);
            void ResetForceAfterImpact();
            Collider *GroundCollider = nullptr;
            //! Movement
            Vector Velocity;
            double Weight = 1;
    };
}

#endif // RIGIDBODY_H
