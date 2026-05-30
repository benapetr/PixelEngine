//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef QOPENGLRENDERER_H
#define QOPENGLRENDERER_H

#include "renderer.h"
#include <QHash>
#include <QOpenGLTexture>
#include <QOpenGLTextureBlitter>

class QOpenGLContext;
class QPaintDevice;
class QPainter;

namespace PE
{
    class QOpenGLRenderer : public Renderer
    {
        public:
            QOpenGLRenderer(int width, int height, QPaintDevice *paintDevice, QOpenGLContext *context);
            ~QOpenGLRenderer() override;
            RendererType GetType() override;
            RendererBackend GetBackend() const override;
            int GetCapabilities() const override;
            void Clear() override;
            void Clear(const QColor &color) override;
            void DrawPixel(int x, int y, const QColor &color) override;
            void DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap) override;
            void DrawLine(Vector source, Vector target, int line_width, const QColor &color) override;
            void DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill = false) override;
            void DrawRoundedRect(int x, int y, int width, int height, int radius, int line_width, const QColor &color, bool fill = false) override;
            void DrawText(int x, int y, const QString &text, const QColor &color, int size = 10) override;
            void DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width=1) override;
            void PushClipRect(int x, int y, int width, int height) override;
            void PopClipRect() override;
            void ClearCaches() override;
            void InvalidateTexture(qint64 cacheKey);
            void Begin();
            void End();
            void SetContext(QOpenGLContext *context) { this->context = context; }

        private:
            bool initializeGLResources();
            void beginPainter();
            void endPainter();
            QOpenGLTexture *textureForPixmap(const QPixmap &pixmap);
            int worldToQtY(int y) const;

            QPaintDevice *paintDevice = nullptr;
            QOpenGLContext *context = nullptr;
            QPainter *painter = nullptr;
            QOpenGLTextureBlitter blitter;
            QHash<qint64, QOpenGLTexture*> textureCache;
            bool glResourcesInitialized = false;
            bool painterActive = false;
    };
}

#endif // QOPENGLRENDERER_H
