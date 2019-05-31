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
