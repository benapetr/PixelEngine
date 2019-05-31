//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
    // In case someone else implements deletion of rigidbody in their own destructor this will
    // prevent shooting in foot
    this->RigidBody = nullptr;
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

void Actor::RemoveChildren(Object *obj)
{
    Object::RemoveChildren(obj);
    if (obj->GetType() == PE_ObjectType_Collider)
        this->colliders.removeAll(dynamic_cast<Collider*>(obj));
}

QList<Collectable_SmartPtr<Collider>> Actor::GetColliders() const
{
    return this->colliders;
}
