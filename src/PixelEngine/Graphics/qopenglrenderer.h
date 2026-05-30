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
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLTextureBlitter>
#include <QPointF>
#include <QRect>
#include <QVector>

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
            RendererStats GetStats() const override;
            void ResetStats() override;
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
            struct DrawCommand
            {
                QOpenGLTexture *Texture = nullptr;
                QRect Rect;
            };

            struct Vertex
            {
                GLfloat X;
                GLfloat Y;
                GLfloat U;
                GLfloat V;
            };

            struct LineVertex
            {
                GLfloat X;
                GLfloat Y;
            };

            bool initializeGLResources();
            void beginPainter();
            void endPainter();
            QOpenGLTexture *textureForPixmap(const QPixmap &pixmap);
            QOpenGLTexture *textureForColor(const QColor &color);
            void queueTextureRect(QOpenGLTexture *texture, int x, int y, int width, int height);
            void flushCommands();
            void flushCommandBatch(QOpenGLTexture *texture, const QVector<DrawCommand> &batch);
            void appendCommandVertices(const DrawCommand &command, QVector<Vertex> *vertices) const;
            bool drawGLLine(Vector source, Vector target, int lineWidth, const QColor &color);
            void appendLineVertices(Vector source, Vector target, int lineWidth, QVector<LineVertex> *vertices) const;
            LineVertex lineVertexFromScreenPoint(float x, float y) const;
            bool drawGLEllipse(int x, int y, int width, int height, const QColor &color, int lineWidth);
            bool drawGLRoundedRect(int x, int y, int width, int height, int radius, int lineWidth, const QColor &color, bool fill);
            bool drawColoredGeometry(const QVector<LineVertex> &vertices, GLenum primitiveMode, const QColor &color);
            void appendRoundedRectRing(int x, int y, int width, int height, int radius, int thickness, QVector<LineVertex> *vertices) const;
            void appendRoundedRectFan(int x, int y, int width, int height, int radius, QVector<LineVertex> *vertices) const;
            QVector<QPointF> roundedRectPoints(float x, float y, float width, float height, float radius) const;
            QVector<QPointF> roundedRectStrokePoints(float x, float y, float width, float height, float radius, float offset) const;
            QPointF roundedRectInnerPoint(const QPointF &point, const QPointF &center, float thickness) const;
            int worldToQtY(int y) const;

            QPaintDevice *paintDevice = nullptr;
            QOpenGLContext *context = nullptr;
            QPainter *painter = nullptr;
            QOpenGLTextureBlitter blitter;
            QOpenGLShaderProgram *textureProgram = nullptr;
            QOpenGLShaderProgram *lineProgram = nullptr;
            QOpenGLBuffer vertexBuffer;
            QHash<qint64, QOpenGLTexture*> textureCache;
            QHash<QRgb, QOpenGLTexture*> colorTextureCache;
            QVector<DrawCommand> commands;
            RendererStats stats;
            bool glResourcesInitialized = false;
            bool painterActive = false;
    };
}

#endif // QOPENGLRENDERER_H
