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
    class CapsuleCollider;
    class CircleCollider;
    class EllipticCollider;
    class LineCollider;
    class PolygonCollider;
    class ColliderMath
    {
        public:
            //! Check if line is going through circle
            static bool IntersectionCheckLineCircle(Vector a, Vector b, CircleCollider *c);
            static bool IntersectionCheckBoxBox(BoxCollider *a, BoxCollider *b);
            static bool IntersectionCheckBoxBitmap(BoxCollider *a, BitmapCollider *b);
            static bool IntersectionCheckBoxCircle(BoxCollider *a, CircleCollider *b);
            static bool IntersectionCheckBoxCapsule(BoxCollider *a, CapsuleCollider *b);
            static bool IntersectionCheckCapsuleBitmap(CapsuleCollider *a, BitmapCollider *b);
            static bool IntersectionCheckCapsuleCapsule(CapsuleCollider *a, CapsuleCollider *b);
            static bool IntersectionCheckCapsuleCircle(CapsuleCollider *a, CircleCollider *b);
            static bool IntersectionCheckEllipseBox(EllipticCollider *a, BoxCollider *b);
            static bool IntersectionCheckEllipseCapsule(EllipticCollider *a, CapsuleCollider *b);
            static bool IntersectionCheckEllipseCircle(EllipticCollider *a, CircleCollider *b);
            static bool IntersectionCheckEllipseEllipse(EllipticCollider *a, EllipticCollider *b);
            static bool IntersectionCheckLineBitmap(LineCollider *a, BitmapCollider *b);
            static bool IntersectionCheckLineBox(LineCollider *a, BoxCollider *b);
            static bool IntersectionCheckLineCapsule(LineCollider *a, CapsuleCollider *b);
            static bool IntersectionCheckLineCircle(LineCollider *a, CircleCollider *b);
            static bool IntersectionCheckLineEllipse(LineCollider *a, EllipticCollider *b);
            static bool IntersectionCheckLineLine(LineCollider *a, LineCollider *b);
            static bool IntersectionCheckPolygonBitmap(PolygonCollider *a, BitmapCollider *b);
            static bool IntersectionCheckPolygonBox(PolygonCollider *a, BoxCollider *b);
            static bool IntersectionCheckPolygonCapsule(PolygonCollider *a, CapsuleCollider *b);
            static bool IntersectionCheckPolygonCircle(PolygonCollider *a, CircleCollider *b);
            static bool IntersectionCheckPolygonEllipse(PolygonCollider *a, EllipticCollider *b);
            static bool IntersectionCheckPolygonLine(PolygonCollider *a, LineCollider *b);
            static bool IntersectionCheckPolygonPolygon(PolygonCollider *a, PolygonCollider *b);
            static bool IntersectionCheckCircleBitmap(BitmapCollider *a, CircleCollider *b);
            static bool IntersectionCheckCircleCircle(CircleCollider *a, CircleCollider *b);
    };
}

#endif // COLLIDERMATH_H
