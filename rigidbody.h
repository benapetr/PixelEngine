// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "vector.h"

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
            void AddForce(Vector force);
            void ResetForceAfterImpact();
            Collider *GroundCollider = nullptr;
            //! Movement
            Vector Velocity;
            double Weight = 1;
            // Even if this doesn't make much sense, it makes the game more fun to play - we set a limit to
            // gravitational force on a world level, so that objects can't fall faster than that, unless they
            // are explicitly powering themselves (rockets), for that reason we need to know if current movement
            // is affected by gravity force, or something else
            double GravityForce = 0;
    };
}

#endif // RIGIDBODY_H
