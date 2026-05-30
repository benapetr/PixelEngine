//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "qopenglrenderer.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QPixmap>

using namespace PE;

QOpenGLRenderer::QOpenGLRenderer(int width, int height, QPaintDevice *paintDevice, QOpenGLContext *context) : Renderer(width, height)
{
    this->paintDevice = paintDevice;
    this->context = context;
}

QOpenGLRenderer::~QOpenGLRenderer()
{
    this->endPainter();
    qDeleteAll(this->textureCache);
    this->textureCache.clear();
    delete this->painter;
}

RendererType QOpenGLRenderer::GetType()
{
    return RendererType_OpenGL;
}

RendererBackend QOpenGLRenderer::GetBackend() const
{
    return RendererBackend_OpenGL;
}

int QOpenGLRenderer::GetCapabilities() const
{
    return RendererCapability_Textures |
           RendererCapability_Text |
           RendererCapability_Clipping |
           RendererCapability_RoundedRects;
}

RendererStats QOpenGLRenderer::GetStats() const
{
    return this->stats;
}

void QOpenGLRenderer::ResetStats()
{
    this->stats = RendererStats();
}

void QOpenGLRenderer::Clear()
{
    this->Clear(Qt::white);
}

void QOpenGLRenderer::Clear(const QColor &color)
{
    this->endPainter();
    if (!this->initializeGLResources())
        return;

    QOpenGLFunctions *f = this->context->functions();
    f->glViewport(0, 0, this->r_width, this->r_height);
    f->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    f->glClear(GL_COLOR_BUFFER_BIT);
    this->stats.Frames++;
}

void QOpenGLRenderer::DrawPixel(int x, int y, const QColor &color)
{
    if (!this->Enabled)
        return;
    this->beginPainter();
    QPen pen(color);
    this->painter->setPen(pen);
    this->painter->drawPoint(x, this->worldToQtY(y));
    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap)
{
    if (!this->Enabled)
        return;

    QOpenGLTexture *texture = this->textureForPixmap(bitmap);
    if (!texture)
        return;

    this->endPainter();
    int qtY = this->worldToQtY(y + height);

    QOpenGLFunctions *f = this->context->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    QRectF targetRect(x, qtY, width, height);
    QRect viewportRect(0, 0, this->r_width, this->r_height);
    QMatrix4x4 transform = QOpenGLTextureBlitter::targetTransform(targetRect, viewportRect);

    this->blitter.bind();
    this->blitter.blit(texture->textureId(), transform, QOpenGLTextureBlitter::OriginTopLeft);
    this->blitter.release();
    this->stats.DrawCalls++;

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawLine(Vector source, Vector target, int line_width, const QColor &color)
{
    if (!this->Enabled)
        return;

    this->beginPainter();
    QPen pen(color);
    pen.setWidth(line_width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    this->painter->setPen(pen);
    this->painter->drawLine(source.X2int(), this->worldToQtY(source.Y2int()), target.X2int(), this->worldToQtY(target.Y2int()));

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill)
{
    if (!this->Enabled)
        return;

    this->beginPainter();
    if (fill)
    {
        QBrush brush(color, Qt::SolidPattern);
        this->painter->fillRect(x, this->worldToQtY(y + height), width, height, brush);
    }
    else
    {
        QPen pen(color);
        pen.setWidth(line_width);
        pen.setCapStyle(Qt::SquareCap);
        pen.setJoinStyle(Qt::MiterJoin);
        this->painter->setPen(pen);
        this->painter->drawRect(x, this->worldToQtY(y + height), width, height);
    }

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawRoundedRect(int x, int y, int width, int height, int radius, int line_width, const QColor &color, bool fill)
{
    if (!this->Enabled)
        return;

    QRectF rect(x, this->worldToQtY(y + height), width, height);
    this->beginPainter();
    this->painter->save();
    this->painter->setRenderHint(QPainter::Antialiasing, true);
    if (fill)
    {
        QBrush brush(color, Qt::SolidPattern);
        this->painter->setPen(Qt::NoPen);
        this->painter->setBrush(brush);
        this->painter->drawRoundedRect(rect, radius, radius, Qt::AbsoluteSize);
    }
    else
    {
        QPen pen(color);
        pen.setWidth(line_width);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        this->painter->setPen(pen);
        this->painter->setBrush(Qt::NoBrush);
        this->painter->drawRoundedRect(rect, radius, radius, Qt::AbsoluteSize);
    }
    this->painter->restore();

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawText(int x, int y, const QString &text, const QColor &color, int size)
{
    if (!this->Enabled)
        return;

    this->beginPainter();
    QPen pen(color);
    QFont font;
    font.setPixelSize(size);
    this->painter->setPen(pen);
    this->painter->setFont(font);
    this->painter->drawText(x, this->worldToQtY(y), text);

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width)
{
    if (!this->Enabled)
        return;

    this->beginPainter();
    QPen pen(color);
    pen.setWidth(line_width);
    this->painter->setPen(pen);
    this->painter->drawEllipse(x, this->worldToQtY(y + height), width, height);

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::PushClipRect(int x, int y, int width, int height)
{
    this->beginPainter();
    this->painter->save();
    this->painter->setClipRect(x, this->worldToQtY(y + height), width, height, Qt::IntersectClip);
}

void QOpenGLRenderer::PopClipRect()
{
    this->beginPainter();
    this->painter->restore();
}

void QOpenGLRenderer::ClearCaches()
{
    this->endPainter();
    qDeleteAll(this->textureCache);
    this->textureCache.clear();
}

void QOpenGLRenderer::InvalidateTexture(qint64 cacheKey)
{
    this->endPainter();
    delete this->textureCache.take(cacheKey);
}

void QOpenGLRenderer::Begin()
{
    this->initializeGLResources();
}

void QOpenGLRenderer::End()
{
    this->endPainter();
}

bool QOpenGLRenderer::initializeGLResources()
{
    if (this->glResourcesInitialized)
        return true;

    if (!this->context || QOpenGLContext::currentContext() != this->context)
        return false;

    if (!this->blitter.isCreated() && !this->blitter.create())
        return false;

    this->glResourcesInitialized = true;
    return true;
}

void QOpenGLRenderer::beginPainter()
{
    if (this->painterActive)
        return;

    if (!this->painter)
        this->painter = new QPainter();
    this->painter->begin(this->paintDevice);
    this->painterActive = true;
    this->stats.PainterFallbacks++;
}

void QOpenGLRenderer::endPainter()
{
    if (!this->painterActive)
        return;

    this->painter->end();
    this->painterActive = false;
}

QOpenGLTexture *QOpenGLRenderer::textureForPixmap(const QPixmap &pixmap)
{
    if (pixmap.isNull() || !this->initializeGLResources())
        return nullptr;

    qint64 key = pixmap.cacheKey();
    if (this->textureCache.contains(key))
    {
        this->stats.TextureCacheHits++;
        return this->textureCache[key];
    }

    QImage image = pixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
    QOpenGLTexture *texture = new QOpenGLTexture(image);
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    this->textureCache.insert(key, texture);
    this->stats.TextureUploads++;
    return texture;
}

int QOpenGLRenderer::worldToQtY(int y) const
{
    return this->r_height - y;
}
