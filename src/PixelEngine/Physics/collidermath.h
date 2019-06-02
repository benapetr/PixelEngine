//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef COLLIDERMATH_H
#define COLLIDERMATH_H

#include "../vector.h"

namespace PE
{
    class BoxCollider;
    class BitmapCollider;
    class CircleCollider;
    class ColliderMath
    {
        public:
            //! Check if line is going through circle
            static bool IntersectionCheckLineCircle(Vector a, Vector b, CircleCollider *c);
            static bool IntersectionCheckBoxBox(BoxCollider *a, BoxCollider *b);
            static bool IntersectionCheckBoxBitmap(BoxCollider *a, BitmapCollider *b);
            static bool IntersectionCheckBoxCircle(BoxCollider *a, CircleCollider *b);
            static bool IntersectionCheckCircleBitmap(BitmapCollider *a, CircleCollider *b);
            static bool IntersectionCheckCircleCircle(CircleCollider *a, CircleCollider *b);
    };
}

#endif // COLLIDERMATH_H
