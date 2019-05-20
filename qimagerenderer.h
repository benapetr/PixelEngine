// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef QIMAGERENDERER_H
#define QIMAGERENDERER_H

#include "renderer.h"
#include <QPixmap>

class QImage;
class QPainter;

namespace PE
{
    class QImageRenderer : public Renderer
    {
        public:
            QImageRenderer(int width, int height);
            ~QImageRenderer() override;
            void Render() override;
            QImage *GetImage();
            void Clear() override;
            void Clear(QColor color) override;
            QPixmap GetPixmap();
            void DrawPixel(int x, int y, QColor color) override;
            void DrawBitmap(int x, int y, int width, int height, QBitmap bitmap) override;
            void DrawLine(Vector source, Vector target, int line_width, QColor color) override;
            void DrawRect(int x, int y, int width, int height, int line_width, QColor color, bool fill = false) override;

        private:
            int trimX(int x);
            int trimY(int y);
            //! Qt has Y other way this function maps the world Y to Qt Y
            int worldToQtY(int y);
            QPainter *painter;
            QImage *image;
    };
}

#endif // QIMAGERENDERER_H
