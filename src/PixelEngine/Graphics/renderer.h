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
#include "../vector.h"

namespace PE
{
    enum RendererType
    {
        RendererType_QImage,
        RendererType_OpenGL
    };

    class Vector;

    /*!
     * \brief The Renderer is abstract class which defines an interface to currently selected platform for rendering
     *
     * It contains interface functions that are using underlying rendering subsystem to draw various shapes and images
     */
    class Renderer
    {
        public:
            Renderer(int width, int height);
            virtual ~Renderer();
            virtual RendererType GetType()=0;
            virtual void Clear()=0;
            virtual void Clear(const QColor &color)=0;
            virtual void DrawPixel(int x, int y, const QColor &color)=0;
            virtual void DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill = false)=0;
            virtual void DrawLine(Vector source, Vector target, int line_width, const QColor &color)=0;
            virtual void DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap)=0;
            virtual void DrawText(int x, int y, const QString &text, const QColor &color, int size = 10)=0;
            virtual void DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width=1)=0;
            int GetHeight() { return this->r_height; }
            int GetWidth() { return this->r_width; }
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
