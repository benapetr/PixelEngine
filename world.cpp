// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "world.h"
#include "actor.h"
#include "camera.h"
#include "Physics/collider.h"
#include "Graphics/renderer.h"
#include "Physics/rigidbody.h"
#include "terrain.h"
#include "Physics/bitmapcollider.h"
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
    this->actors.clear();
    this->objects.clear();
    this->terrains.clear();
    this->colliders.clear();
    delete this->camera;
}

void World::Render(Renderer *r)
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
    }

    if (!this->redrawNeeded)
        return;

    this->redrawNeeded = false;

    r->Clear(this->BackgroundColor);

    // terrains
    foreach (Terrain *t, this->terrains)
    {
        t->Render(r, this->camera);
    }

    // layers
    QList<int> indexes = this->objects.keys();
    qSort(indexes);

    foreach (int i, indexes)
    {
        foreach (Object *x, this->objects[i])
        {
            if (x->IsVisibleOnCamera(this->camera))
                x->Render(r, this->camera);
            x->RedrawNeeded = false;
        }
    }
    if (r->ManualUpdate)
        r->HasUpdate = true;

    if (this->HasBorder)
        r->DrawRect(0, 0, static_cast<int>(this->world_width - 1), static_cast<int>(this->world_height - 1), 6, QColor("blue"));
}

void World::Update()
{
    this->lastUpdate = this->GetTime();
    this->deleteOld();
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

qint64 World::GetTime()
{
    return QDateTime::currentDateTime().toMSecsSinceEpoch();
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
            Object *collision_source = nullptr;

            foreach (Collider *collider_actor, ac)
            {
                foreach (Collider *collider_other, all_colliders)
                {
                    if (collider_actor == collider_other)
                        continue;
                    if (collider_actor->GetParent() == collider_other->GetParent())
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

                a->Event_OnImpact(a->RigidBody->Velocity);

                // Now let's try to find a position between the source and target colider, to prevent any space gaps in between them
                a->RigidBody->ResetForceAfterImpact();

                // Spawn the event
                a->Event_OnCollision(collision_target);
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
