//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

namespace PE
{
    class Camera
    {
        public:
            Camera();
            Vector GetNegated();
            //! Position relative to world from current camera point of view
            Vector ProjectedPosition(Vector position);
            Vector Position;
    };
}

#endif // CAMERA_H
