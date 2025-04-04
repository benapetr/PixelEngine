//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019 - 2024

#ifndef QGLRENDERER_H
#define QGLRENDERER_H

#include "renderer.h"
#include <QOpenGLFunctions>
#include <QOpenGLTextureBlitter>
#include <QOpenGLTexture>
#include <QImage>

class QOpenGLWidget;

namespace PE
{
    class QGLRenderer : public Renderer
    {
        public:
            QGLRenderer(int width, int height, QPaintDevice *widget, QOpenGLContext *gl_context);
            ~QGLRenderer() override;
            RendererType GetType() override;
            void Clear() override;
            void Clear(const QColor &color) override;
            void DrawPixel(int x, int y, const QColor &color) override;
            void DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap) override;
            void DrawLine(Vector source, Vector target, int line_width, const QColor &color) override;
            void DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill = false) override;
            void DrawText(int x, int y, const QString &text, const QColor &color, int size = 10) override;
            void DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width=1) override;
            void Begin();
            void End();
            //! This is experimental and doesn't seem to work
            //! Low level OpenGL API that loads image into GPU memory and returns its ID for further use
            GLuint LoadTexture(const QImage &image);
            //! This is much faster than regular Draw Bitmap utilizing power of the GPU
            void DrawBitmap(int x, int y, int width, int height, GLuint textureID);
            void DrawTexture(int x, int y, int width, int height, QOpenGLTexture *texture);
            void SetContext(QOpenGLContext *gc) { this->context = gc; }

        private:
            //! Qt has Y other way this function maps the world Y to Qt Y
            int worldToQtY(int y);
            QOpenGLTextureBlitter blitter;
            QOpenGLContext *context;
            QPaintDevice *paintDevice;
            QPainter *painter = nullptr;
    };
}

#endif // QGLRENDERER_H
