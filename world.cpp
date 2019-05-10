//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "world.h"
#include "actor.h"
#include "renderer.h"
#include "rigidbody.h"

using namespace PE;

World::World(double width, double height)
{
    this->world_width = width;
    this->world_height = height;
}

World::~World()
{
    this->actors.clear();
    QList<int> indexes = this->objects.keys();
    foreach (int i, indexes)
    {
        foreach (Object *x, this->objects[i])
        {
            delete x;
        }
    }
    this->objects.clear();
}

void World::Render(Renderer *r)
{
    r->Clear();

    if (this->HasBorder)
        r->DrawRect(0, 0, static_cast<int>(this->world_width - 1), static_cast<int>(this->world_height - 1), 6, QColor("blue"));

    // layers
    QList<int> indexes = this->objects.keys();
    qSort(indexes);

    foreach (int i, indexes)
    {
        foreach (Object *x, this->objects[i])
        {
            x->Render(r);
        }
    }
    if (r->ManualUpdate)
        r->HasUpdate = true;
}

void World::Update()
{
    this->updatePhysics();
    foreach (Actor *a, this->actors)
    {
        a->Update();
    }
}

void World::updatePhysics()
{
    this->updateGravity();
    this->updateMovement();
}

void World::RegisterActor(Actor *a, int zindex)
{
    if (!this->objects.contains(zindex))
        this->objects.insert(zindex, QList<Object*>());
    this->objects[zindex].append(a);
    this->actors.append(a);
}

void World::updateGravity()
{
    // Calculate gravitional forces for every object with rigidbody
    foreach (Actor *a, this->actors)
    {
        if (!a->RigidBody)
            continue;

        if (a->RigidBody->IsGrounded())
            continue;

        a->RigidBody->GravityForce += this->Gravity * a->RigidBody->Weight;
        if (a->RigidBody->GravityForce > this->GravityMax)
            a->RigidBody->GravityForce = this->GravityMax;
    }
}

void World::updateMovement()
{
    foreach (Actor *a, this->actors)
    {
        if (!a->RigidBody)
            continue;

        Vector movement = a->RigidBody->GetMovement();

        a->Position.X += movement.X;
        a->Position.Y += movement.Y;
    }
}
