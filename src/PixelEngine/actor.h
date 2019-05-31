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

    /*!
     * \brief The Actor class is a base for all actors - objects that can be affected by world physics (in case they have some rigidbody)
     *
     * Actors are low level elements from which the world consist of. Each actor can have collisions, rigidbody (if it's supposed to be
     * dynamically affected by physics subsystem), can render graphics and has many events. There is a virtual function Update which is called
     * every single frame of world update (don't confuse with renderer frames - FPS).
     */
    class Actor : public Object
    {
        public:
            Actor();
            Actor(const Vector &position, Object *parent = nullptr);
            ~Actor() override;
            PE_ObjectType GetType() override;
            void AddChildren(Object *obj) override;
            void RemoveChildren(Object *obj) override;
            virtual void Event_KeyPress(int key) { (void)key; }
            virtual void Event_KeyRelease(int key) { (void)key; }
            virtual void Update(qint64 time = 0) { (void)time; }
            virtual void Event_OnImpact(const Vector &impact_force) { (void)impact_force; }
            QList<Collectable_SmartPtr<Collider>> GetColliders() const;
            // Only actors with rigidbody have any physics applied to them
            Rigidbody *RigidBody = nullptr;

        protected:
            // List of colliders bound to this actor, this is used as a cache only
            QList<Collectable_SmartPtr<Collider>> colliders;
    };
}

#endif // ACTOR_H
