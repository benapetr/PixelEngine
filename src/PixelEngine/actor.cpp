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
#include "Serialization/classregistry.h"
#include "Serialization/deserializer.h"
#include "Serialization/jsondeserializer.h"
#include "Serialization/jsonserializer.h"
#include "Serialization/serializer.h"
#include <QJsonObject>
#include <QVariantList>

using namespace PE;

Actor::Actor()
{

}

Actor::Actor(const Vector &position, Object *parent) : Object(position, parent)
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

QString Actor::GetClassName() const
{
    return "PE::Actor";
}

void Actor::Serialize(Serializer *serializer) const
{
    Object::Serialize(serializer);

    if (this->RigidBody)
    {
        JsonSerializer rigidBodySerializer;
        this->RigidBody->Serialize(&rigidBodySerializer);
        serializer->WriteValue("rigidBody", rigidBodySerializer.ToJsonObject().toVariantMap());
    }

    QVariantList colliderList;
    foreach (Collider *collider, this->colliders)
    {
        JsonSerializer colliderSerializer;
        collider->Serialize(&colliderSerializer);
        colliderList.append(colliderSerializer.ToJsonObject().toVariantMap());
    }
    serializer->WriteValue("colliders", colliderList);
}

void Actor::Deserialize(Deserializer *deserializer)
{
    Object::Deserialize(deserializer);

    QVariantMap rigidBodyMap = deserializer->ReadValue("rigidBody").toMap();
    if (!rigidBodyMap.isEmpty())
    {
        Serializable *rigidBody = ClassRegistry::GetRegistry()->Create(rigidBodyMap.value("class").toString());
        if (rigidBody)
        {
            JsonDeserializer rigidBodyDeserializer(QJsonObject::fromVariantMap(rigidBodyMap));
            rigidBody->Deserialize(&rigidBodyDeserializer);
            delete this->RigidBody;
            this->RigidBody = dynamic_cast<Rigidbody*>(rigidBody);
            if (!this->RigidBody)
                delete rigidBody;
        }
    }

    QVariantList colliderList = deserializer->ReadValue("colliders").toList();
    foreach (const QVariant &colliderValue, colliderList)
    {
        QVariantMap colliderMap = colliderValue.toMap();
        Serializable *serializable = ClassRegistry::GetRegistry()->Create(colliderMap.value("class").toString());
        if (!serializable)
            continue;

        JsonDeserializer colliderDeserializer(QJsonObject::fromVariantMap(colliderMap));
        serializable->Deserialize(&colliderDeserializer);
        Object *object = dynamic_cast<Object*>(serializable);
        if (object)
            this->AddChildren(object);
        else
            delete serializable;
    }
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
