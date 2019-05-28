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
            enum PainterMode
            {
                PainterMode_Destructing,
                PainterMode_Creating,
                PainterMode_CreatingSpecial
            };
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
            void DestroyPixel(int x, int y);
            void CreatePixel(int x, int y);
            void DestroyPixelAbsolute(int x, int y);
            void CreatePixelAbsolute(int x, int y);
            void RefreshPixmap();
            //! Shift all bits that are above bottom but doesn't have any other bits under down by one pixel
            //! this is extremely CPU expensive function
            //! Returns number of pixels shifted
            int ShiftFloatingBitsDownByOnePixel();
            //! Shift all bits that are above bottom but doesn't have any other bits under down completely until they hit ground
            //! this is extremely CPU expensive function
            //! Returns number of pixels shifted
            int ShiftFloatingBitsDown();
            Collectable_SmartPtr<BitmapCollider> Collider;
            QPixmap BitMap;
            QImage SourceImage;
            QColor TerrainColor;
            QColor BackgroundColor;

        protected:
            int terrainToQtY(int y);
            int t_width;
            int t_height;
            QPainter *getPainter();
            void changePainterMode(PainterMode mode);
            QPainter *painter = nullptr;
            PainterMode painterMode = PainterMode_Destructing;
    };
}

#endif // TERRAIN_H
