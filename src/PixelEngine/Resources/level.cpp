//! \file level.cpp
//! \brief Implementation of serializable level definitions.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "level.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include "../world.h"
#include <QVariantList>

using namespace PE;

QVariantMap LevelPlacement::ToVariantMap() const
{
    QVariantMap map;
    QVariantMap position;
    position.insert("x", this->Position.X);
    position.insert("y", this->Position.Y);
    map.insert("id", this->Id);
    map.insert("package", this->PackagePath);
    map.insert("entry", this->EntryId);
    map.insert("position", position);
    map.insert("rotation", this->Rotation);
    map.insert("scale", this->Scale);
    map.insert("zIndex", this->ZIndex);
    return map;
}

LevelPlacement LevelPlacement::FromVariantMap(const QVariantMap &map)
{
    LevelPlacement placement;
    QVariantMap position = map.value("position").toMap();
    placement.Id = map.value("id").toString();
    placement.PackagePath = map.value("package").toString();
    placement.EntryId = map.value("entry").toString();
    placement.Position = Vector(position.value("x").toDouble(), position.value("y").toDouble());
    placement.Rotation = static_cast<pe_float_t>(map.value("rotation", placement.Rotation).toDouble());
    placement.Scale = static_cast<pe_float_t>(map.value("scale", placement.Scale).toDouble());
    placement.ZIndex = map.value("zIndex", placement.ZIndex).toInt();
    return placement;
}

Level::Level()
{

}

Level::Level(const QString &id, const QString &name, pe_float_t width, pe_float_t height)
{
    this->Id = id;
    this->Name = name;
    this->Width = width;
    this->Height = height;
}

QString Level::GetClassName() const
{
    return "PE::Level";
}

void Level::Serialize(Serializer *serializer) const
{
    QVariantList placements;
    for (const LevelPlacement &placement : this->Placements)
        placements.append(placement.ToVariantMap());

    serializer->WriteString("class", this->GetClassName());
    serializer->WriteString("id", this->Id);
    serializer->WriteString("name", this->Name);
    serializer->WriteString("description", this->Description);
    serializer->WriteFloat("width", this->Width);
    serializer->WriteFloat("height", this->Height);
    serializer->WriteFloat("gravity", this->Gravity);
    serializer->WriteString("backgroundColor", this->BackgroundColor.name(QColor::HexArgb));
    serializer->WriteString("borderColor", this->BorderColor.name(QColor::HexArgb));
    serializer->WriteBool("hasBorder", this->HasBorder);
    serializer->WriteValue("placements", placements);
}

void Level::Deserialize(Deserializer *deserializer)
{
    this->Id = deserializer->ReadString("id", this->Id);
    this->Name = deserializer->ReadString("name", this->Name);
    this->Description = deserializer->ReadString("description", this->Description);
    this->Width = deserializer->ReadFloat("width", this->Width);
    this->Height = deserializer->ReadFloat("height", this->Height);
    this->Gravity = deserializer->ReadFloat("gravity", this->Gravity);
    this->BackgroundColor = QColor(deserializer->ReadString("backgroundColor", this->BackgroundColor.name(QColor::HexArgb)));
    this->BorderColor = QColor(deserializer->ReadString("borderColor", this->BorderColor.name(QColor::HexArgb)));
    this->HasBorder = deserializer->ReadBool("hasBorder", this->HasBorder);
    this->Placements.clear();
    QVariantList placements = deserializer->ReadValue("placements").toList();
    for (const QVariant &placement : placements)
        this->Placements.append(LevelPlacement::FromVariantMap(placement.toMap()));
}

World *Level::CreateWorld() const
{
    World *world = new World(this->Width, this->Height);
    world->Gravity = this->Gravity;
    world->BackgroundColor = this->BackgroundColor;
    world->BorderColor = this->BorderColor;
    world->HasBorder = this->HasBorder;
    return world;
}
