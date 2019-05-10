//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef ACTOR_H
#define ACTOR_H

#include "object.h"

namespace PE
{
    class Rigidbody;
    class Actor : public Object
    {
        public:
            Actor();
            ~Actor();
            PE_ObjectType GetType() override;
            virtual void Update() {}
            // Only actors with rigidbody have any physics applied to them
            Rigidbody *RigidBody = nullptr;
    };
}

#endif // ACTOR_H
