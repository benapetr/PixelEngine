//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "collisionignorematrix.h"

using namespace PE;

CollisionIgnoreMatrix::CollisionIgnoreMatrix()
{
    int i = PE_MAX_CM_SIZE;
    while (i-- > 0)
        this->Matrix.append(std::bitset<PE_MAX_CM_SIZE>());
}

void CollisionIgnoreMatrix::SetMutualIgnoreShip(unsigned int x, unsigned int y)
{
    this->Matrix[x][y] = true;
    this->Matrix[y][x] = true;
}
