//! \file worldgenerator.h
//! \brief Header file for the WorldGenerator class, responsible for generating terrains in the game world.

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

    //! \class WorldGenerator
    //! \brief Provides functionality to generate terrains for the game world.
    class WorldGenerator
    {
        public:
            //! \brief Generate a random terrain.
            //! \param w_width The width of the terrain.
            //! \param w_height The height of the terrain.
            //! \param background The background color of the terrain.
            //! \param terrain_color The color of the terrain.
            //! \return A smart pointer to the generated Terrain object.
            static Collectable_SmartPtr<Terrain> GenerateRandom(int w_width, int w_height, const QColor &background, const QColor &terrain_color);
    };
}

#endif // WORLDGENERATOR_H
