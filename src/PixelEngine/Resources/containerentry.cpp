//! \file containerentry.cpp
//! \brief Implementation of generic asset container entries.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "containerentry.h"
#include <QJsonDocument>
#include <QJsonObject>

using namespace PE;

ContainerEntry::ContainerEntry()
{

}

QString ContainerEntry::EntryTypeToString(PE_ContainerEntryType type)
{
    switch (type)
    {
        case PE_ContainerEntryType_SerializedObject:
            return "SerializedObject";
        case PE_ContainerEntryType_BinaryBlob:
            return "BinaryBlob";
        case PE_ContainerEntryType_AssetReference:
            return "AssetReference";
        case PE_ContainerEntryType_Container:
            return "Container";
        case PE_ContainerEntryType_Unknown:
        default:
            return "Unknown";
    }
}

PE_ContainerEntryType ContainerEntry::EntryTypeFromString(const QString &type)
{
    if (type == "SerializedObject")
        return PE_ContainerEntryType_SerializedObject;
    if (type == "BinaryBlob")
        return PE_ContainerEntryType_BinaryBlob;
    if (type == "AssetReference")
        return PE_ContainerEntryType_AssetReference;
    if (type == "Container")
        return PE_ContainerEntryType_Container;
    return PE_ContainerEntryType_Unknown;
}

QVariantMap ContainerEntry::ToVariantMap() const
{
    QVariantMap map;
    map.insert("id", this->Id);
    map.insert("name", this->Name);
    map.insert("description", this->Description);
    map.insert("type", ContainerEntry::EntryTypeToString(this->Type));
    map.insert("class", this->ClassName);
    map.insert("encoding", this->Encoding);
    map.insert("metadata", this->Metadata);

    if (!this->ReferenceId.isEmpty())
        map.insert("reference", this->ReferenceId);

    if (this->Type == PE_ContainerEntryType_SerializedObject && this->Encoding == "json")
    {
        QJsonDocument document = QJsonDocument::fromJson(this->Data);
        if (document.isObject())
        {
            map.insert("object", document.object().toVariantMap());
            return map;
        }
    }

    if (!this->Data.isEmpty())
        map.insert("data", QString::fromLatin1(this->Data.toBase64()));

    return map;
}

ContainerEntry ContainerEntry::FromVariantMap(const QVariantMap &map)
{
    ContainerEntry entry;
    entry.Id = map.value("id").toString();
    entry.Name = map.value("name").toString();
    entry.Description = map.value("description").toString();
    entry.Type = ContainerEntry::EntryTypeFromString(map.value("type").toString());
    entry.ClassName = map.value("class").toString();
    entry.Encoding = map.value("encoding").toString();
    entry.ReferenceId = map.value("reference").toString();
    entry.Metadata = map.value("metadata").toMap();

    if (map.contains("object"))
    {
        QJsonObject object = QJsonObject::fromVariantMap(map.value("object").toMap());
        entry.Data = QJsonDocument(object).toJson(QJsonDocument::Compact);
        if (entry.Encoding.isEmpty())
            entry.Encoding = "json";
    } else if (map.contains("data"))
    {
        entry.Data = QByteArray::fromBase64(map.value("data").toString().toLatin1());
    }

    return entry;
}
