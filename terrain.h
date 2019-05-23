// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef TERRAIN_H
#define TERRAIN_H

#include "object.h"
#include <QPixmap>
#include <QImage>

class QPainter;

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
            //! Will replace pixel in underlying SourceImage with BackgroundColor and also removes the bit in collision map
            //! this function is optimized for high speed, after you are done removing all pixels you need, you also have
            //! to call RefreshPixmap for graphical changes to take effect
            void DestroyPixel(Vector p);
            void RefreshPixmap();
            Collectable_SmartPtr<BitmapCollider> Collider;
            QPixmap BitMap;
            QImage SourceImage;
            QColor BackgroundColor;

        protected:
            int terrainToQtY(int y);
            int t_width;
            int t_height;
            QPainter *getPainter();
            QPainter *painter = nullptr;
    };
}

#endif // TERRAIN_H
