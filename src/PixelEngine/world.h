//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef WORLD_H
#define WORLD_H

#include "definitions.h"
#include "GC/collectable_smartptr.h"
#include <QHash>
#include <QColor>

namespace PE
{
    class Renderer;
    class Actor;
    class Camera;
    class CollisionIgnoreMatrix;
    class Object;
    class Terrain;
    class Collider;

    /*!
     * \brief The World class is engine base for a world - containing all world objects - actors, collisions and terrains.
     *
     * World implements engine physics and is also responsible for rendering of whole scene.
     *
     * Every component of the engine should be a part of some world, each world has its own rules of physics.
     */
    class World
    {
        public:
            World(double width, double height);
            virtual ~World();
            //! Check if redraw is needed and if yes, it will draw a whole world using provided renderer, if redraw is not needed (no change)
            //! function returns false
            bool Render(Renderer *r);
            void ForceRender(Renderer *r);
            //! Execute a world frame - that is a basic unit that controls game speed
            //!
            //! This function should be periodically executed either by a timer or in a loop using separate thread
            void Update();
            void RegisterActor(Actor *a, int zindex = 1);
            void RegisterObject(Object *o, int zindex = 1);
            void RegisterTerrain(Terrain *t, int zindex = 0);
            void RegisterCollider(Collider *c);
            //! Immediately destroy object and remove it from world
            void DestroyObject(Collectable_SmartPtr<Object> o);
            void ProcessKeyPress(int key);
            void ProcessKeyRelease(int key);
            //! Return all colliders registered withing the world, including colliders belonging to actors and terrain
            QList<Collectable_SmartPtr<Collider>> GetAllWorldColliders();
            qint64 GetTime();
            bool HasCollisionIgnoreMatrix();
            CollisionIgnoreMatrix *GetCollisionIgnoreMatrix();
            bool HasBorder = true;
            QColor BorderColor;
            // Every object that makes it under this Y is automatically destroyed
            // this is here to handle players that fall behind terrain
            double DeathZone = -20;
            double Gravity = 1;
            //double GravityMax = 10;
            QColor BackgroundColor;

        protected:
            void updatePhysics();
            void updateGravity();
            void updateMovement();
            void deleteOld();

        private:
            CollisionIgnoreMatrix *CIM = nullptr;
            Camera *camera;
            QList<Collectable_SmartPtr<Actor>> actors;
            QList<Collectable_SmartPtr<Collider>> colliders;
            QHash<int, QList<Collectable_SmartPtr<Object>>> objects;
            QList<Collectable_SmartPtr<Terrain>> terrains;
            double world_width;
            double world_height;
            bool redrawNeeded;
            qint64 lastUpdate = 0;
    };
}

#endif // WORLD_H
