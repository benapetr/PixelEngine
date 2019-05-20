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
#include "GC/collectable_smartptr.h"
#include <QList>

namespace PE
{
    class Rigidbody;
    class Collider;
    class Actor : public Object
    {
        public:
            Actor();
            ~Actor() override;
            PE_ObjectType GetType() override;
            void AddChildren(Object *obj) override;
            virtual void Event_KeyPress(int key) { (void)key; }
            virtual void Event_KeyRelease(int key) { (void)key; }
            virtual void Update(qint64 time = 0) { (void)time; }
            QList<Collectable_SmartPtr<Collider>> GetColliders() const;
            // Only actors with rigidbody have any physics applied to them
            Rigidbody *RigidBody = nullptr;

        protected:
            // List of colliders bound to this actor, this is used as a cache only
            QList<Collectable_SmartPtr<Collider>> colliders;
    };
}

#endif // ACTOR_H
