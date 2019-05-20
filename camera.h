// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
