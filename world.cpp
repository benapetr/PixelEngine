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
#include "collider.h"
#include "renderer.h"
#include "rigidbody.h"
#include "terrain.h"
#include "bitmapcollider.h"
#include <QDateTime>

using namespace PE;

World::World(double width, double height)
{
    this->BackgroundColor = Qt::white;
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
    qDeleteAll(this->colliders);
    qDeleteAll(this->terrains);
}

void World::Render(Renderer *r)
{
    r->Clear(this->BackgroundColor);

    // terrains
    foreach (Terrain *t, this->terrains)
    {
        t->Render(r);
    }

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

    if (this->HasBorder)
        r->DrawRect(0, 0, static_cast<int>(this->world_width - 1), static_cast<int>(this->world_height - 1), 6, QColor("blue"));
}

void World::Update()
{
    this->lastUpdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
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

void World::RegisterTerrain(Terrain *t, int zindex)
{
    this->terrains.append(t);
    this->colliders.append(t->Collider);
}

void World::RegisterCollider(Collider *c)
{
    this->colliders.append(c);
}

void World::updateGravity()
{
    // Calculate gravitional forces for every object with rigidbody
    foreach (Actor *a, this->actors)
    {
        if (!a->RigidBody)
            continue;

        if (a->RigidBody->IsGrounded() && a->LastMovementUpdate >= a->RigidBody->GroundCollider->LastMovementUpdate)
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

        Vector current_velocity = a->RigidBody->GetMovement();
        if (current_velocity == Vector::Zero)
            continue;

        // Check if this actor has some colliders, if yes, we need to check if there is a collision caused by this movement
        // if yes, don't move
        Vector old_position = a->Position;
        a->SetPosition(a->Position + current_velocity);

        QList<Collider*> ac = a->GetColliders();
        if (!ac.isEmpty())
        {
            // Get a list of all colliders in world (terrain colliders + other (actor) colliders)
            QList<Collider*> all_colliders = this->colliders;
            foreach (Actor *foreign_actor, this->actors)
            {
                if (foreign_actor == a)
                    continue;
                all_colliders.append(foreign_actor->GetColliders());
            }

            Collider *collision_target = nullptr;
            Collider *collision_source = nullptr;

            foreach (Collider *collider_actor, ac)
            {
                foreach (Collider *collider_other, all_colliders)
                {
                    if (collider_actor == collider_other)
                        continue;
                    if (collider_actor->GetParent() == collider_other->GetParent())
                        continue;
                    if (collider_actor->IntersectionMatch(collider_other))
                    {
                        // There is a collision
                        collision_target = collider_other;
                        collision_source = collider_actor;
                        break;
                    }
                }
                if (collision_target)
                    break;
            }
            if (collision_target)
            {
                // In case that collision is under the object (or blocking it from falling down), let's ground the object
                // we don't do this check if object is moving to side
                if (current_velocity.X == 0 && current_velocity.Y < 0 && (old_position.Y - a->Position.Y) <= 0.5)
                {
                    a->RigidBody->GroundCollider = collision_target;
                    // Update cache times
                    a->LastMovementUpdate = this->lastUpdate;
                    collision_target->LastMovementUpdate = this->lastUpdate;
                }
                collision_source->Event_OnCollision(collision_target);
                // We can't move this object, let's move it back
                a->SetPosition(old_position);

                // Now let's try to find a position between the source and target colider, to prevent any space gaps in between them
                a->RigidBody->ResetForceAfterImpact();
            } else
            {
                // The object has moved, let's update the physics cache so that all objects that are related to this one will know it
                a->UpdateRecursivelyLastMovement(this->lastUpdate);
            }
        }
    }
}
