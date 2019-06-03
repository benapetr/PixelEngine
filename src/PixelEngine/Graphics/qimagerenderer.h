//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
            QImage *GetImage();
            RendererType GetType() override;
            void Clear() override;
            void Clear(const QColor &color) override;
            QPixmap GetPixmap();
            void DrawPixel(int x, int y, const QColor &color) override;
            void DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap) override;
            void DrawLine(Vector source, Vector target, int line_width, const QColor &color) override;
            void DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill = false) override;
            void DrawText(int x, int y, const QString &text, const QColor &color, int size = 10) override;
            void DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width=1) override;

        private:
            //! Qt has Y other way this function maps the world Y to Qt Y
            int worldToQtY(int y);
            QPainter *painter;
            QImage *image;
    };
}

#endif // QIMAGERENDERER_H
