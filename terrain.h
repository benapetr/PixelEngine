// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef TERRAIN_H
#define TERRAIN_H

#include "object.h"
#include <QBitmap>

namespace PE
{
    class BitmapCollider;
    class Collider;
    class Terrain : public Object
    {
        public:
            Terrain(double x, double y, int width, int height);
            ~Terrain() override;
            PE_ObjectType GetType() override;
            void Render(Renderer *r, Camera *c) override;
            void SetPosition(Vector p) override;
            Collectable_SmartPtr<BitmapCollider> Collider;
            QBitmap BitMap;
    };
}

#endif // TERRAIN_H
