// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
            static Collectable_SmartPtr<Terrain> GenerateRandom(int w_width, int w_height);
    };
}

#endif // WORLDGENERATOR_H
