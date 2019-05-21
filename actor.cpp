// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "actor.h"
#include "Physics/collider.h"
#include "Physics/rigidbody.h"

using namespace PE;

Actor::Actor()
{

}

Actor::~Actor()
{
    delete this->RigidBody;
}

PE_ObjectType Actor::GetType()
{
    return PE_ObjectType_Actor;
}

void Actor::AddChildren(Object *obj)
{
    Object::AddChildren(obj);
    if (obj->GetType() == PE_ObjectType_Collider)
        this->colliders.append(dynamic_cast<Collider*>(obj));
}

QList<Collectable_SmartPtr<Collider>> Actor::GetColliders() const
{
    return this->colliders;
}
