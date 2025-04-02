//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

//! \file pawn.h
//! \brief Header file for the Pawn class, a specialized type of Actor in Pixel Engine.

#ifndef PAWN_H
#define PAWN_H

#include "actor.h"

namespace PE
{
    //! \class Pawn
    //! \brief A specialized type of Actor in the Pixel Engine.
    //! 
    //! Pawns are dynamic objects in the game world that can be controlled or influenced by external factors.
    class Pawn : public Actor
    {
        public:
            //! \brief Default constructor for the Pawn class.
            Pawn();
    };
}

#endif // PAWN_H
