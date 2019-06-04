//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef BITMAPCOLLIDER_H
#define BITMAPCOLLIDER_H

#include "../definitions.h"
#include "collider.h"
#include <bitset>

typedef std::bitset<PE_BITSET_MAX> pe_bitset;

namespace PE
{
    class ColliderMath;

    //! Bitmap collision - for obvious reasons (memory) works with integers only
    //! smallest collision unit is one integer (one pixel)
    //! This collider doesn't scale
    class BitmapCollider : public Collider
    {
        public:
            BitmapCollider(int x, int y, int w, int h, Object *parent = nullptr);
            bool PositionMatch(Vector position) override;
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Bitmap; }
            bool IntersectionMatch(Collider *collider) override;
            int GetWidth() { return this->width; }
            int GetHeight() { return this->height; }
            QList<pe_bitset> Bitmap;
        private:
            int width;
            int height;

            friend class ColliderMath;
    };
}

#endif // BITMAPCOLLIDER_H
