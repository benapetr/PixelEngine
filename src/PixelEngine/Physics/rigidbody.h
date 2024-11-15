//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../definitions.h"
#include "../vector.h"
#include "collider.h"
#include "../GC/collectable_smartptr.h"

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
            Collectable_SmartPtr<Collider> GroundCollider;
            //! Movement
            Vector Velocity;
            pe_float_t Bounciness = 0;
            pe_float_t Weight = 1;
    };
}

#endif // RIGIDBODY_H
