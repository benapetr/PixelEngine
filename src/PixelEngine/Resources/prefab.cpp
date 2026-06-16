//! \file prefab.cpp
//! \brief Implementation of reusable object prefabs.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "prefab.h"
#include "../object.h"
#include "../Serialization/classregistry.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/jsondeserializer.h"
#include "../Serialization/jsonserializer.h"
#include "../Serialization/serializer.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantList>

using namespace PE;

static Object *CreatePrefabObjectFromMap(const QString &fallbackClassName, const QVariantMap &objectMap)
{
    QString className = objectMap.value("class", fallbackClassName).toString();
    if (className.isEmpty())
        return nullptr;

    Serializable *serializable = ClassRegistry::GetRegistry()->Create(className);
    Object *object = dynamic_cast<Object*>(serializable);
    if (!object)
    {
        delete serializable;
        return nullptr;
    }

    QJsonDocument document(QJsonObject::fromVariantMap(objectMap));
    JsonDeserializer deserializer(document.toJson(QJsonDocument::Compact));
    if (!deserializer.IsValid())
    {
        delete object;
        return nullptr;
    }

    object->Deserialize(&deserializer);

    QVariantList children = objectMap.value("children").toList();
    for (const QVariant &child : children)
    {
        Object *childObject = CreatePrefabObjectFromMap(QString(), child.toMap());
        if (childObject)
            object->AddChildren(childObject);
    }

    return object;
}

Prefab::Prefab()
{

}

Prefab::Prefab(const QString &id, const QString &name, const Object *root)
{
    this->Id = id;
    this->Name = name;
    if (root)
        this->SetRootObject(root);
}

QString Prefab::GetClassName() const
{
    return "PE::Prefab";
}

void Prefab::Serialize(Serializer *serializer) const
{
    serializer->WriteString("class", this->GetClassName());
    serializer->WriteString("id", this->Id);
    serializer->WriteString("name", this->Name);
    serializer->WriteString("description", this->Description);
    serializer->WriteString("rootClass", this->RootClassName);
    serializer->WriteValue("root", this->RootObject);
}

void Prefab::Deserialize(Deserializer *deserializer)
{
    this->Id = deserializer->ReadString("id", this->Id);
    this->Name = deserializer->ReadString("name", this->Name);
    this->Description = deserializer->ReadString("description", this->Description);
    this->RootClassName = deserializer->ReadString("rootClass", this->RootClassName);
    this->RootObject = deserializer->ReadValue("root").toMap();
}

void Prefab::SetRootObject(const Object *root)
{
    if (!root)
        return;

    JsonSerializer serializer;
    root->Serialize(&serializer);
    QJsonDocument document = QJsonDocument::fromJson(serializer.ToJson(QJsonDocument::Compact));
    this->RootClassName = root->GetClassName();
    this->RootObject = document.object().toVariantMap();
}

Object *Prefab::CreateObject() const
{
    if (this->RootClassName.isEmpty() || this->RootObject.isEmpty())
        return nullptr;

    return CreatePrefabObjectFromMap(this->RootClassName, this->RootObject);
}
