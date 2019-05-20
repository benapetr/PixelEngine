// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
    class Object;
    class Terrain;
    class Collider;

    /*!
     * \brief The World class is engine base for a world - containing all actors and terrains.
     *
     * Every component of the engine should be a part of some world, each world has its own rules of physics
     */
    class World
    {
        public:
            World(double width, double height);
            virtual ~World();
            void Render(Renderer *r);
            void Update();
            void RegisterActor(Actor *a, int zindex = 1);
            void RegisterObject(Object *o, int zindex = 1);
            void RegisterTerrain(Terrain *t, int zindex = 0);
            void RegisterCollider(Collider *c);
            void ProcessKeyPress(int key);
            void ProcessKeyRelease(int key);
            bool HasBorder = true;
            // Every object that makes it under this Y is automatically destroyed
            // this is here to handle players that fall behind terrain
            double DeathZone = -20;
            double Gravity = 1;
            double GravityMax = 10;
            QColor BackgroundColor;

        protected:
            void updatePhysics();
            void updateGravity();
            void updateMovement();

        private:
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
