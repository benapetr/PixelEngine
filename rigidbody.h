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
            Collider *GroundCollider = nullptr;
            Vector Movement;
            double Weight = 1;
            // Even if this doesn't make much sense, it makes the game more fun to play - we set a limit to
            // gravitational force on a world level, so that objects can't fall faster than that, unless they
            // are explicitly powering themselves (rockets), for that reason we need to know if current movement
            // is affected by gravity force, or something else
            double GravityForce = 0;
    };
}

#endif // RIGIDBODY_H
