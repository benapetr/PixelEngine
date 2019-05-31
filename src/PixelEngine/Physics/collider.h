//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
