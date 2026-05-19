//! \file assetcontainer.cpp
//! \brief Implementation of generic asset containers.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "assetcontainer.h"
#include "../object.h"
#include "../Serialization/classregistry.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/jsondeserializer.h"
#include "../Serialization/jsonserializer.h"
#include "../Serialization/serializer.h"
#include <QFile>
#include <QJsonDocument>
#include <QVariantList>

using namespace PE;

AssetContainer::AssetContainer()
{

}

AssetContainer::AssetContainer(const QString &id, const QString &name, const QString &description)
{
    this->Id = id;
    this->Name = name;
    this->Description = description;
}

QString AssetContainer::GetClassName() const
{
    return "PE::AssetContainer";
}

void AssetContainer::Serialize(Serializer *serializer) const
{
    QVariantList entries;
    foreach (const ContainerEntry &entry, this->Entries)
        entries.append(entry.ToVariantMap());

    serializer->WriteString("class", this->GetClassName());
    serializer->WriteString("id", this->Id);
    serializer->WriteString("name", this->Name);
    serializer->WriteString("description", this->Description);
    serializer->WriteValue("entries", entries);
}

void AssetContainer::Deserialize(Deserializer *deserializer)
{
    this->Id = deserializer->ReadString("id", this->Id);
    this->Name = deserializer->ReadString("name", this->Name);
    this->Description = deserializer->ReadString("description", this->Description);
    this->Entries.clear();

    QVariantList entries = deserializer->ReadValue("entries").toList();
    foreach (const QVariant &entry, entries)
        this->Entries.append(ContainerEntry::FromVariantMap(entry.toMap()));
}

void AssetContainer::AddEntry(const ContainerEntry &entry)
{
    this->RemoveEntry(entry.Id);
    this->Entries.append(entry);
}

ContainerEntry *AssetContainer::GetEntry(const QString &id)
{
    for (int i = 0; i < this->Entries.size(); ++i)
    {
        if (this->Entries[i].Id == id)
            return &this->Entries[i];
    }
    return nullptr;
}

const ContainerEntry *AssetContainer::GetEntry(const QString &id) const
{
    for (int i = 0; i < this->Entries.size(); ++i)
    {
        if (this->Entries[i].Id == id)
            return &this->Entries[i];
    }
    return nullptr;
}

bool AssetContainer::HasEntry(const QString &id) const
{
    return this->GetEntry(id) != nullptr;
}

void AssetContainer::RemoveEntry(const QString &id)
{
    for (int i = this->Entries.size() - 1; i >= 0; --i)
    {
        if (this->Entries[i].Id == id)
            this->Entries.removeAt(i);
    }
}

void AssetContainer::AddBinary(const QString &id, const QString &name, const QByteArray &data, const QString &encoding, const QString &description)
{
    ContainerEntry entry;
    entry.Id = id;
    entry.Name = name;
    entry.Description = description;
    entry.Encoding = encoding;
    entry.Data = data;
    entry.Type = PE_ContainerEntryType_BinaryBlob;
    this->AddEntry(entry);
}

void AssetContainer::AddReference(const QString &id, const QString &name, const QString &referenceId, const QString &description)
{
    ContainerEntry entry;
    entry.Id = id;
    entry.Name = name;
    entry.Description = description;
    entry.ReferenceId = referenceId;
    entry.Type = PE_ContainerEntryType_AssetReference;
    this->AddEntry(entry);
}

void AssetContainer::AddSerializable(const QString &id, const QString &name, const Serializable *object, const QString &description)
{
    JsonSerializer serializer;
    object->Serialize(&serializer);

    ContainerEntry entry;
    entry.Id = id;
    entry.Name = name;
    entry.Description = description;
    entry.ClassName = object->GetClassName();
    entry.Encoding = "json";
    entry.Data = serializer.ToJson(QJsonDocument::Compact);
    entry.Type = PE_ContainerEntryType_SerializedObject;
    this->AddEntry(entry);
}

Serializable *AssetContainer::CreateSerializable(const QString &id) const
{
    const ContainerEntry *entry = this->GetEntry(id);
    if (!entry || entry->Type != PE_ContainerEntryType_SerializedObject || entry->Encoding != "json")
        return nullptr;

    Serializable *object = ClassRegistry::GetRegistry()->Create(entry->ClassName);
    if (!object)
        return nullptr;

    JsonDeserializer deserializer(entry->Data);
    if (!deserializer.IsValid())
    {
        delete object;
        return nullptr;
    }

    object->Deserialize(&deserializer);
    return object;
}

Object *AssetContainer::CreateObject(const QString &id) const
{
    return dynamic_cast<Object*>(this->CreateSerializable(id));
}

bool AssetContainer::SaveToFile(const QString &path) const
{
    JsonSerializer serializer;
    this->Serialize(&serializer);

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write(serializer.ToJson(QJsonDocument::Indented));
    file.close();
    return true;
}

AssetContainer *AssetContainer::LoadFromFile(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return nullptr;

    QByteArray data = file.readAll();
    file.close();

    JsonDeserializer deserializer(data);
    if (!deserializer.IsValid())
        return nullptr;

    AssetContainer *container = new AssetContainer();
    container->Deserialize(&deserializer);
    return container;
}
