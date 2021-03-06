//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include "GC/collectable_smartptr.h"
#include "terrain.h"

namespace PE
{
    class Terrain;
    class WorldGenerator
    {
        public:
            static Collectable_SmartPtr<Terrain> GenerateRandom(int w_width, int w_height, const QColor &background, const QColor &terrain_color);
    };
}

#endif // WORLDGENERATOR_H
