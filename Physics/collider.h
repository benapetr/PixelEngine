// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef COLLIDER_H
#define COLLIDER_H

#include "../object.h"

namespace PE
{
    enum PE_ColliderType
    {
        PE_ColliderType_Pixel,
        PE_ColliderType_Box,
        PE_ColliderType_Bitmap,
        PE_ColliderType_Ellipse,
        PE_ColliderType_Circle
    };

    class Collider : public Object
    {
        public:
#ifdef PE_DEBUG
            static bool Debug;
#endif

            Collider(Object *parent = nullptr);
            // Returns true in case that position is inside body of this collider
            virtual bool PositionMatch(Vector position)=0;
            virtual bool IntersectionMatch(Collider *collider)=0;
            virtual PE_ColliderType GetColliderType()=0;
            PE_ObjectType GetType() override;
    };
}

#endif // COLLIDER_H
