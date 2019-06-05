//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef COLLISIONIGNOREMATRIX_H
#define COLLISIONIGNOREMATRIX_H

#include "../definitions.h"
#include <QList>
#include <bitset>

namespace PE
{
    //! Provides simple and quick mechanism to define whether collision layers are to be ignored or not
    //! for performance reasons direct access to Matrix - bypassing the function call is allowed
    class CollisionIgnoreMatrix
    {
        public:
            CollisionIgnoreMatrix();
            void SetMutualIgnoreShip(unsigned int x, unsigned int y);
            bool Ignored(unsigned int x, unsigned int y) { return this->Matrix[x][y]; }
            QList<std::bitset<PE_MAX_CM_SIZE>> Matrix;
    };
}

#endif // COLLISIONIGNOREMATRIX_H
