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

#include "collider.h"

namespace PE
{
    //! Bitmap collision - for obvious reasons (memory) works with integers only
    //! smallest collision unit is one integer (one pixel)
    class BitmapCollider : public Collider
    {
        public:
            BitmapCollider(int x, int y, int w, int h);
            bool PositionMatch(Vector position) override;
            ColliderType GetColliderType() override { return ColliderType_Bitmap; }
            bool IntersectionMatch(Collider *collider) override;
            int GetWidth() { return this->width; }
            int GetHeight() { return this->height; }
        private:
            int X;
            int Y;
            int width;
            int height;
            char **bitmap;
    };
}

#endif // BITMAPCOLLIDER_H
