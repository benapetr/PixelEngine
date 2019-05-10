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

namespace PE
{
    class QImageRenderer : public Renderer
    {
        public:
            QImageRenderer(int width, int height);
            void Render() override;
            QImage *GetImage();
            void Clear() override;
            QPixmap GetPixmap();
            void DrawPixel(int x, int y, QColor color) override;
            void DrawRect(int x, int y, int width, int height, int line_width, QColor color) override;

        private:
            int trimX(int x);
            int trimY(int y);
            //! Qt has Y other way this function maps the world Y to Qt Y
            int worldToQtY(int y);
            QImage *image;
    };
}

#endif // QIMAGERENDERER_H
