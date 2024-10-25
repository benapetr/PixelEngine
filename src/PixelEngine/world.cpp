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
#include "camera.h"
#include "Physics/collider.h"
#include "Physics/collisionignorematrix.h"
#include "Graphics/renderer.h"
#include "Physics/rigidbody.h"
#include "terrain.h"
#include "Physics/bitmapcollider.h"
#include <cmath>
#include <QDateTime>

using namespace PE;

World::World(double width, double height)
{
    this->BackgroundColor = Qt::white;
    this->world_width = width;
    this->world_height = height;
    this->redrawNeeded = true;
    this->camera = new Camera();
}

World::~World()
{
    foreach (Actor *a, this->actors)
        a->DestroyNow();
    this->actors.clear();
    this->objects.clear();
    this->terrains.clear();
    this->colliders.clear();
    delete this->camera;
    delete this->CIM;
}

bool World::Render(Renderer *r)
{
    if (!this->redrawNeeded)
    {
        foreach (Actor *a, this->actors)
        {
            if (a->RedrawNeeded)
            {
                if (a->IsVisibleOnCamera(this->camera))
                {
                    this->redrawNeeded = true;
                    break;
                } else
                {
                    a->RedrawNeeded = false;
                }
            }
        }
        foreach(Terrain *t, this->terrains)
        {
            if (t->RedrawNeeded)
            {
                t->RedrawNeeded = false;
                this->redrawNeeded = true;
            }
        }
    }

    if (!this->redrawNeeded)
        return false;

    this->redrawNeeded = false;

    r->Clear(this->BackgroundColor);

    // terrains
    foreach (Terrain *t, this->terrains)
    {
        t->Render(r, this->camera);
    }

    // layers
    QList<int> indexes = this->objects.keys();
    std::sort(indexes.begin(), indexes.end());

    foreach (int i, indexes)
    {
        foreach (Object *x, this->objects[i])
        {
            if (x->IsVisibleOnCamera(this->camera))
                x->Render(r, this->camera);
            x->RedrawNeeded = false;
        }
    }
#ifdef PE_DEBUG
    // Display colliders on camera
    if (Collider::Debug)
    {
        foreach (Collider *c, this->GetAllWorldColliders())
        {
            if (c->IsVisibleOnCamera(this->camera))
                c->Render(r, this->camera);
        }
    }
#endif
    if (r->ManualUpdate)
        r->HasUpdate = true;

    if (this->HasBorder)
        r->DrawRect(0, 0, static_cast<int>(this->world_width - 1), static_cast<int>(this->world_height - 1), 6, this->BorderColor);

    return true;
}

void World::ForceRender(Renderer *r)
{
    this->redrawNeeded = true;
    this->Render(r);
}

void World::Update()
{
    this->lastUpdate = this->GetTime();
    this->deleteOld();
    this->updatePhysics();
    foreach (Actor *a, this->actors)
    {
        a->Update(this->lastUpdate);
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
        this->objects.insert(zindex, QList<Collectable_SmartPtr<Object>>());
    this->objects[zindex].append(a);
    this->actors.append(a);
    this->redrawNeeded = true;
}

void World::RegisterObject(Object *o, int zindex)
{
    if (!this->objects.contains(zindex))
        this->objects.insert(zindex, QList<Collectable_SmartPtr<Object>>());
    this->objects[zindex].append(o);
}

void World::RegisterTerrain(Terrain *t, int zindex)
{
    this->terrains.append(t);
    this->colliders.append(dynamic_cast<Collider*>(t->Collider.GetPtr()));
    this->redrawNeeded = true;
}

void World::RegisterCollider(Collider *c)
{
    this->colliders.append(c);
}

void World::DestroyObject(Collectable_SmartPtr<Object> o)
{
    o->DestroyNow();
    if (o->GetType() == PE_ObjectType_Actor)
    {
        Collectable_SmartPtr<Actor> a(dynamic_cast<Actor*>(o.GetPtr()));
        this->actors.removeAll(a);
    }
    QList<int> indexes = this->objects.keys();
    foreach (int i, indexes)
    {
        this->objects[i].removeAll(o);
    }
    this->redrawNeeded = true;
}

void World::ProcessKeyPress(int key)
{
    foreach (Actor *a, this->actors)
        a->Event_KeyPress(key);
}

void World::ProcessKeyRelease(int key)
{
    foreach (Actor *a, this->actors)
        a->Event_KeyRelease(key);
}

QList<Collectable_SmartPtr<Collider> > World::GetAllWorldColliders()
{
    QList<Collectable_SmartPtr<Collider>> all_colliders = this->colliders;
    foreach (Actor *foreign_actor, this->actors)
    {
        all_colliders.append(foreign_actor->GetColliders());
    }
    return all_colliders;
}

qint64 World::GetTime()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
}

bool World::HasCollisionIgnoreMatrix()
{
    return (nullptr == this->CIM);
}

CollisionIgnoreMatrix *World::GetCollisionIgnoreMatrix()
{
    if (!this->CIM)
        this->CIM = new CollisionIgnoreMatrix();
    return this->CIM;
}

void World::updateGravity()
{
    // Calculate gravitional forces for every object with rigidbody
    foreach (Actor *a, this->actors)
    {
        if (!a->RigidBody)
            continue;

        if (a->RigidBody->IsGrounded())
        {
            if (a->LastMovementUpdate >= a->RigidBody->GroundCollider->LastMovementUpdate)
                continue;
            else
                a->RigidBody->GroundCollider = nullptr;
        }
        a->RigidBody->Velocity.Y += -1 * this->Gravity * a->RigidBody->Weight;
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

        QList<Collectable_SmartPtr<Collider>> ac = a->GetColliders();
        if (!ac.isEmpty())
        {
            // Get a list of all colliders in world (terrain colliders + other (actor) colliders)
            QList<Collectable_SmartPtr<Collider>> all_colliders = this->colliders;
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
                    if (this->CIM && this->CIM->Matrix[collider_actor->Layer][collider_other->Layer])
                        continue;

                    // Ray tracing hack for pixel colliders - should be improved
                    if (collider_actor->GetColliderType() == PE_ColliderType_Pixel)
                    {
                        // Only ray trace if speed is worth it
                        if (a->RigidBody->Velocity.X > 2 || a->RigidBody->Velocity.Y > 2)
                        {
                            double distance = old_position.DistanceTo(a->Position);
                            Vector step = a->RigidBody->Velocity / distance;
                            int current_step = 0;
                            while (++current_step < static_cast<int>(distance))
                            {
                                Vector cp = a->Position + (step * current_step);
                                if (collider_other->PositionMatch(cp))
                                {
                                    collision_target = collider_other;
                                    collision_source = collider_actor;
                                    // Because we did a pixel ray trace and hit something, we know exactly where is last pixel
                                    // where we didn't hit it. So we can shift the old_position to that place, so that object
                                    // is moved right in front of whatever is there
                                    old_position = a->Position + (step * (current_step-1));
                                    goto loop_exit;
                                }
                            }
                        }
                    }

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

            loop_exit:

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

                PE::Vector impact_force = a->RigidBody->Velocity;
                a->RigidBody->ResetForceAfterImpact();

                // Events
                a->Event_OnImpact(impact_force);
                a->Event_OnCollision(collision_target);

                // If object bounces, let's bounce it
                if (a->RigidBody->Bounciness > 0 && std::abs(impact_force.Magnitude()) > 1)
                {
                    bool hit_y;
                    bool hit_x;
                    // But first check in which direction it did impact - because that's the direction we want to negate
                    Vector original_position_of_collider = collision_source->Position;

                    // Now shift it in both direction and check if there is collision caused by either shift
                    collision_source->Position.Y += impact_force.Y;
                    hit_y = collision_target->IntersectionMatch(collision_source);

                    collision_source->Position = original_position_of_collider;
                    collision_source->Position.X += impact_force.X;
                    hit_x = collision_target->IntersectionMatch(collision_source);

                    collision_source->Position = original_position_of_collider;
                    if (hit_x || hit_y)
                    {
                        Vector bounce_force = impact_force;
                        if (hit_x)
                            bounce_force.X = impact_force.X * -1 * a->RigidBody->Bounciness;
                        if (hit_y)
                            bounce_force.Y = impact_force.Y * -1 * a->RigidBody->Bounciness;
                        a->RigidBody->AddForce(bounce_force);
                    }
                }
            } else
            {
                // The object has moved, let's update the physics cache so that all objects that are related to this one will know it
                a->UpdateRecursivelyLastMovement(this->lastUpdate);
                this->redrawNeeded = true;
            }
        } else if (old_position != a->Position)
        {
            this->redrawNeeded = true;
        }
    }
}

void World::deleteOld()
{
    foreach (Actor *a, this->actors)
    {
        if (a->DestroyAfter < 0)
            continue;
        if (a->DestroyAfter < this->lastUpdate)
            this->DestroyObject(a);
    }
}
