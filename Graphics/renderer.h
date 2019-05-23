// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef RENDERER_H
#define RENDERER_H

#include <QColor>
#include <QBitmap>
#include "../vector.h"

namespace PE
{
    class Vector;
    class Renderer
    {
        public:
            Renderer(int width, int height);
            virtual ~Renderer();
            virtual void Render()=0;
            virtual void Clear()=0;
            virtual void Clear(QColor color)=0;
            virtual void DrawPixel(int x, int y, QColor color)=0;
            virtual void DrawRect(int x, int y, int width, int height, int line_width, QColor color, bool fill = false)=0;
            virtual void DrawLine(Vector source, Vector target, int line_width, QColor color)=0;
            virtual void DrawBitmap(int x, int y, int width, int height, QPixmap bitmap)=0;
            virtual void DrawText(int x, int y, QString text, QColor color, int size = 10)=0;
            virtual void DrawEllipse(int x, int y, int width, int height, QColor color, int line_width=1)=0;
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
