//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "navigation.h"
#include "pathnode.h"
#include "../../Physics/collider.h"
#include "../../world.h"
#include <cmath>

using namespace PE;

Navigation *Navigation::defaultNavigation = nullptr;
bool Navigation::debug = false;

Navigation *Navigation::GetDefault()
{
    if (!Navigation::defaultNavigation)
        Navigation::defaultNavigation = new Navigation();

    return Navigation::defaultNavigation;
}

void Navigation::SetDebug(bool enabled)
{
    Navigation::debug = enabled;
    PathNode::Debug = enabled;
}

bool Navigation::IsDebug()
{
    return Navigation::debug;
}

void Navigation::RegisterNode(PathNode *node)
{
    if (!node || this->nodes.contains(node))
        return;

    this->nodes.append(node);
    this->dirty = true;
}

void Navigation::UnregisterNode(PathNode *node)
{
    this->nodes.removeAll(node);
    this->paths.remove(node);
    QList<PathNode*> keys = this->paths.keys();
    foreach (PathNode *key, keys)
        this->paths[key].removeAll(node);
    this->dirty = true;
}

void Navigation::Clear()
{
    this->nodes.clear();
    this->paths.clear();
    this->dirty = false;
}

void Navigation::BuildPaths()
{
    this->paths.clear();
    foreach (PathNode *node, this->nodes)
        this->paths.insert(node, QList<PathNode*>());

    for (int i = 0; i < this->nodes.size(); ++i)
    {
        PathNode *source = this->nodes[i];
        for (int j = i + 1; j < this->nodes.size(); ++j)
        {
            PathNode *target = this->nodes[j];
            if (!this->CanConnect(source, target))
                continue;

            this->paths[source].append(target);
            this->paths[target].append(source);
        }
    }

    this->dirty = false;
}

void Navigation::Rebuild()
{
    this->BuildPaths();
}

bool Navigation::IsDirty() const
{
    return this->dirty;
}

QList<PathNode*> Navigation::GetNodes() const
{
    return this->nodes;
}

QList<PathNode*> Navigation::GetNeighbors(PathNode *node) const
{
    return this->paths.value(node);
}

bool Navigation::CanConnect(PathNode *source, PathNode *target) const
{
    if (!source || !target || source == target)
        return false;

    pe_float_t distance = source->Position.DistanceTo(target->Position);
    pe_float_t maxDistance = qMin(source->MaxDistance, target->MaxDistance);
    if (distance > maxDistance)
        return false;

    if (!source->RespectColliders && !target->RespectColliders)
        return true;

    World *world = source->GetWorld();
    if (!world || world != target->GetWorld())
        return true;

    QList<Collectable_SmartPtr<Collider>> colliders = world->GetAllWorldColliders();
    int steps = static_cast<int>(std::ceil(distance / 4.0));
    if (steps <= 1)
        return true;

    Vector delta = target->Position - source->Position;
    for (int step = 1; step < steps; ++step)
    {
        pe_float_t t = static_cast<pe_float_t>(step) / static_cast<pe_float_t>(steps);
        Vector point = source->Position + (delta * t);
        foreach (Collider *collider, colliders)
        {
            if (!collider->BlocksNavigation)
                continue;

            if (collider->PositionMatch(point))
                return false;
        }
    }

    return true;
}
