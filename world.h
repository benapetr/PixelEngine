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

#include <QHash>

namespace PE
{
    class Renderer;
    class Actor;
    class Object;
    class Terrain;
    class World
    {
        public:
            World(double width, double height);
            ~World();
            void Render(Renderer *r);
            void Update();
            void RegisterActor(Actor *a, int zindex = 1);
            void RegisterObject(Object *o, int zindex = 1);
            void RegisterTerrain(Terrain *t, int zindex = 0);
            bool HasBorder = true;
            // Every object that makes it under this Y is automatically destroyed
            // this is here to handle players that fall behind terrain
            double DeathZone = -20;
            double Gravity = 1;
            double GravityMax = 10;

        protected:
            void updatePhysics();
            void updateGravity();
            void updateMovement();

        private:
            QList<Actor*> actors;
            QHash<int, QList<Object*>> objects;
            QHash<int, QList<Terrain*>> terrains;
            double world_width;
            double world_height;
    };
}

#endif // WORLD_H
