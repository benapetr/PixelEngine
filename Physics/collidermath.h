// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef COLLIDERMATH_H
#define COLLIDERMATH_H

namespace PE
{
    class BoxCollider;
    class BitmapCollider;
    class CircleCollider;
    class ColliderMath
    {
        public:
            static bool IntersectionCheckBoxBox(BoxCollider *a, BoxCollider *b);
            static bool IntersectionCheckBoxBitmap(BoxCollider *a, BitmapCollider *b);
            static bool IntersectionCheckBoxCircle(BoxCollider *a, CircleCollider *b);
            static bool IntersectionCheckCircleBitmap(BitmapCollider *a, CircleCollider *b);
            static bool IntersectionCheckCircleCircle(CircleCollider *a, CircleCollider *b);
    };
}

#endif // COLLIDERMATH_H
