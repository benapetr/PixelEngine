//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "pathnode.h"
#include "navigation.h"
#include "../../Graphics/renderer.h"
#include "../../Serialization/deserializer.h"
#include "../../Serialization/serializer.h"
#include "../../camera.h"
#include <QColor>
#include <QtGlobal>

using namespace PE;

bool PathNode::Debug = false;

PathNode::PathNode() : Object()
{
}

PathNode::PathNode(const Vector &position, Object *parent) : Object(position, parent)
{
}

QString PathNode::GetClassName() const
{
    return "PE::PathNode";
}

void PathNode::Serialize(Serializer *serializer) const
{
    Object::Serialize(serializer);
    serializer->WriteFloat("maxDistance", this->MaxDistance);
    serializer->WriteBool("respectColliders", this->RespectColliders);
}

void PathNode::Deserialize(Deserializer *deserializer)
{
    Object::Deserialize(deserializer);
    this->MaxDistance = deserializer->ReadFloat("maxDistance", this->MaxDistance);
    this->RespectColliders = deserializer->ReadBool("respectColliders", this->RespectColliders);
}

void PathNode::Render(Renderer *r, Camera *c)
{
    if (!PathNode::Debug)
        return;

    this->PaintPaths(r, c);
    this->Paint(r, c);
}

void PathNode::Paint(Renderer *r, Camera *c)
{
    Vector position = c->ProjectedPosition(this->Position);
    r->DrawEllipse(position.X2int() - 4, position.Y2int() - 4, 8, 8, QColor(255, 214, 84), 2);
    r->DrawEllipse(position.X2int() - 2, position.Y2int() - 2, 4, 4, QColor(255, 214, 84), 1);
}

void PathNode::PaintPaths(Renderer *r, Camera *c)
{
    QList<PathNode*> neighbors = Navigation::GetDefault()->GetNeighbors(this);
    foreach (PathNode *neighbor, neighbors)
    {
        if (reinterpret_cast<quintptr>(neighbor) < reinterpret_cast<quintptr>(this))
            continue;

        Vector source = c->ProjectedPosition(this->Position);
        Vector target = c->ProjectedPosition(neighbor->Position);
        r->DrawLine(source, target, 1, QColor(255, 214, 84, 150));
    }
}

void PathNode::Event_RegisteredToWorld(World *world)
{
    this->world = world;
    Navigation::GetDefault()->RegisterNode(this);
}

void PathNode::Event_UnregisteredFromWorld(World *world)
{
    (void)world;
    this->world = nullptr;
    Navigation::GetDefault()->UnregisterNode(this);
}

void PathNode::Event_Destroyed()
{
    this->world = nullptr;
    Navigation::GetDefault()->UnregisterNode(this);
}

World *PathNode::GetWorld() const
{
    return this->world;
}
