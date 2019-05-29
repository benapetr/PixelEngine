//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
            //! This is not accurate unless you recalculate it after each change - number of pixels
            int TerrainMass;
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
