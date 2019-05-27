// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019
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
