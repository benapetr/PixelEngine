//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef RENDERER_H
#define RENDERER_H

#include <QColor>
#include <QBitmap>
#include "vector.h"

namespace PE
{
    class Vector;
    class Renderer
    {
        public:
            Renderer();
            virtual ~Renderer();
            virtual void Render()=0;
            virtual void Clear()=0;
            virtual void Clear(QColor color)=0;
            virtual void DrawPixel(int x, int y, QColor color)=0;
            virtual void DrawRect(int x, int y, int width, int height, int line_width, QColor color, bool fill = false)=0;
            virtual void DrawLine(Vector source, Vector target, int line_width, QColor color)=0;
            virtual void DrawBitmap(int x, int y, int width, int height, QBitmap bitmap)=0;
            //! If true the underlying image has changed and should be redrawn to the screen
            bool HasUpdate = false;
            bool ManualUpdate = false;
            bool Enabled = true;
        protected:
            int r_height;
            int r_width;
    };
}

#endif // RENDERER_H
