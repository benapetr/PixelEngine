// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
