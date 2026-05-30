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
#include <QImage>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QPainter>
#include <QPointF>
#include <QPixmap>
#include <algorithm>
#include <cmath>

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
    qDeleteAll(this->colorTextureCache);
    this->colorTextureCache.clear();
    delete this->textureProgram;
    delete this->lineProgram;
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
           RendererCapability_RoundedRects |
           RendererCapability_Batching;
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
    this->flushCommands();
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

    this->queueTextureRect(texture, x, y, width, height);

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QOpenGLRenderer::DrawLine(Vector source, Vector target, int line_width, const QColor &color)
{
    if (!this->Enabled)
        return;

    if (this->drawGLLine(source, target, line_width, color))
    {
        if (!this->ManualUpdate)
            this->HasUpdate = true;
        return;
    }

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

    if (fill)
    {
        QOpenGLTexture *texture = this->textureForColor(color);
        if (texture)
        {
            this->queueTextureRect(texture, x, y, width, height);
            if (!this->ManualUpdate)
                this->HasUpdate = true;
            return;
        }
    }

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

    if (this->drawGLRoundedRect(x, y, width, height, radius, line_width, color, fill))
    {
        if (!this->ManualUpdate)
            this->HasUpdate = true;
        return;
    }

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

    if (this->drawGLEllipse(x, y, width, height, color, line_width))
    {
        if (!this->ManualUpdate)
            this->HasUpdate = true;
        return;
    }

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
    qDeleteAll(this->colorTextureCache);
    this->colorTextureCache.clear();
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
    this->flushCommands();
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

    if (!this->textureProgram)
    {
        this->textureProgram = new QOpenGLShaderProgram();
        const char *vertexShader =
                "attribute vec2 position;\n"
                "attribute vec2 texCoord;\n"
                "varying vec2 vTexCoord;\n"
                "void main() {\n"
                "    gl_Position = vec4(position, 0.0, 1.0);\n"
                "    vTexCoord = texCoord;\n"
                "}\n";
        const char *fragmentShader =
#ifdef GL_ES
                "precision mediump float;\n"
#endif
                "uniform sampler2D textureSampler;\n"
                "varying vec2 vTexCoord;\n"
                "void main() {\n"
                "    gl_FragColor = texture2D(textureSampler, vTexCoord);\n"
                "}\n";

        if (!this->textureProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader) ||
            !this->textureProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader) ||
            !this->textureProgram->link())
        {
            delete this->textureProgram;
            this->textureProgram = nullptr;
            return false;
        }
    }

    if (!this->lineProgram)
    {
        this->lineProgram = new QOpenGLShaderProgram();
        const char *vertexShader =
                "attribute vec2 position;\n"
                "void main() {\n"
                "    gl_Position = vec4(position, 0.0, 1.0);\n"
                "}\n";
        const char *fragmentShader =
#ifdef GL_ES
                "precision mediump float;\n"
#endif
                "uniform vec4 lineColor;\n"
                "void main() {\n"
                "    gl_FragColor = lineColor;\n"
                "}\n";

        if (!this->lineProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader) ||
            !this->lineProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader) ||
            !this->lineProgram->link())
        {
            delete this->lineProgram;
            this->lineProgram = nullptr;
            return false;
        }
    }

    if (!this->vertexBuffer.isCreated())
        this->vertexBuffer.create();

    this->glResourcesInitialized = true;
    return true;
}

void QOpenGLRenderer::beginPainter()
{
    if (this->painterActive)
        return;

    this->flushCommands();
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

QOpenGLTexture *QOpenGLRenderer::textureForColor(const QColor &color)
{
    if (!this->initializeGLResources())
        return nullptr;

    QRgb key = color.rgba();
    if (this->colorTextureCache.contains(key))
        return this->colorTextureCache[key];

    QImage image(1, 1, QImage::Format_RGBA8888);
    image.fill(color);
    QOpenGLTexture *texture = new QOpenGLTexture(image);
    texture->setMinificationFilter(QOpenGLTexture::Nearest);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    this->colorTextureCache.insert(key, texture);
    return texture;
}

void QOpenGLRenderer::queueTextureRect(QOpenGLTexture *texture, int x, int y, int width, int height)
{
    if (!texture || !this->initializeGLResources())
        return;

    this->endPainter();

    DrawCommand command;
    command.Texture = texture;
    command.Rect = QRect(x, y, width, height);
    this->commands.append(command);
}

void QOpenGLRenderer::flushCommands()
{
    if (this->commands.isEmpty())
        return;

    this->endPainter();
    if (!this->initializeGLResources() || !this->textureProgram)
    {
        this->commands.clear();
        return;
    }

    QOpenGLTexture *batchTexture = nullptr;
    QVector<DrawCommand> batch;
    foreach (const DrawCommand &command, this->commands)
    {
        if (!batch.isEmpty() && command.Texture != batchTexture)
        {
            this->flushCommandBatch(batchTexture, batch);
            batch.clear();
        }

        batchTexture = command.Texture;
        batch.append(command);
    }

    if (!batch.isEmpty())
        this->flushCommandBatch(batchTexture, batch);

    this->commands.clear();
}

void QOpenGLRenderer::flushCommandBatch(QOpenGLTexture *texture, const QVector<DrawCommand> &batch)
{
    if (!texture || batch.isEmpty())
        return;

    QVector<Vertex> vertices;
    vertices.reserve(batch.size() * 6);
    foreach (const DrawCommand &command, batch)
        this->appendCommandVertices(command, &vertices);

    QOpenGLFunctions *f = this->context->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    texture->bind(0);
    this->textureProgram->bind();
    this->textureProgram->setUniformValue("textureSampler", 0);

    this->vertexBuffer.bind();
    this->vertexBuffer.allocate(vertices.constData(), vertices.size() * static_cast<int>(sizeof(Vertex)));

    int positionLocation = this->textureProgram->attributeLocation("position");
    int texCoordLocation = this->textureProgram->attributeLocation("texCoord");
    this->textureProgram->enableAttributeArray(positionLocation);
    this->textureProgram->enableAttributeArray(texCoordLocation);
    this->textureProgram->setAttributeBuffer(positionLocation, GL_FLOAT, offsetof(Vertex, X), 2, sizeof(Vertex));
    this->textureProgram->setAttributeBuffer(texCoordLocation, GL_FLOAT, offsetof(Vertex, U), 2, sizeof(Vertex));

    f->glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    this->textureProgram->disableAttributeArray(positionLocation);
    this->textureProgram->disableAttributeArray(texCoordLocation);
    this->vertexBuffer.release();
    this->textureProgram->release();
    texture->release();
    this->stats.DrawCalls++;
}

void QOpenGLRenderer::appendCommandVertices(const DrawCommand &command, QVector<Vertex> *vertices) const
{
    GLfloat left = (static_cast<GLfloat>(command.Rect.x()) / this->r_width) * 2.0f - 1.0f;
    GLfloat right = (static_cast<GLfloat>(command.Rect.x() + command.Rect.width()) / this->r_width) * 2.0f - 1.0f;
    GLfloat bottom = (static_cast<GLfloat>(command.Rect.y()) / this->r_height) * 2.0f - 1.0f;
    GLfloat top = (static_cast<GLfloat>(command.Rect.y() + command.Rect.height()) / this->r_height) * 2.0f - 1.0f;

    vertices->append(Vertex{left, top, 0.0f, 0.0f});
    vertices->append(Vertex{left, bottom, 0.0f, 1.0f});
    vertices->append(Vertex{right, bottom, 1.0f, 1.0f});
    vertices->append(Vertex{left, top, 0.0f, 0.0f});
    vertices->append(Vertex{right, bottom, 1.0f, 1.0f});
    vertices->append(Vertex{right, top, 1.0f, 0.0f});
}

bool QOpenGLRenderer::drawGLLine(Vector source, Vector target, int lineWidth, const QColor &color)
{
    if (!this->initializeGLResources() || !this->lineProgram)
        return false;

    this->flushCommands();
    this->endPainter();

    QVector<LineVertex> vertices;
    vertices.reserve(6);
    this->appendLineVertices(source, target, lineWidth, &vertices);
    if (vertices.isEmpty())
        return true;

    QOpenGLFunctions *f = this->context->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->lineProgram->bind();
    this->lineProgram->setUniformValue("lineColor", QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));

    this->vertexBuffer.bind();
    this->vertexBuffer.allocate(vertices.constData(), vertices.size() * static_cast<int>(sizeof(LineVertex)));

    int positionLocation = this->lineProgram->attributeLocation("position");
    this->lineProgram->enableAttributeArray(positionLocation);
    this->lineProgram->setAttributeBuffer(positionLocation, GL_FLOAT, offsetof(LineVertex, X), 2, sizeof(LineVertex));

    f->glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    this->lineProgram->disableAttributeArray(positionLocation);
    this->vertexBuffer.release();
    this->lineProgram->release();
    this->stats.DrawCalls++;
    return true;
}

void QOpenGLRenderer::appendLineVertices(Vector source, Vector target, int lineWidth, QVector<LineVertex> *vertices) const
{
    float sourceX = source.X;
    float sourceY = source.Y;
    float targetX = target.X;
    float targetY = target.Y;
    float dx = targetX - sourceX;
    float dy = targetY - sourceY;
    float length = std::sqrt((dx * dx) + (dy * dy));
    if (length <= 0)
        return;

    float halfWidth = std::max(1, lineWidth) / 2.0f;
    float normalX = (-dy / length) * halfWidth;
    float normalY = (dx / length) * halfWidth;

    LineVertex a = this->lineVertexFromScreenPoint(sourceX + normalX, sourceY + normalY);
    LineVertex b = this->lineVertexFromScreenPoint(sourceX - normalX, sourceY - normalY);
    LineVertex c = this->lineVertexFromScreenPoint(targetX - normalX, targetY - normalY);
    LineVertex d = this->lineVertexFromScreenPoint(targetX + normalX, targetY + normalY);

    vertices->append(a);
    vertices->append(b);
    vertices->append(c);
    vertices->append(a);
    vertices->append(c);
    vertices->append(d);
}

QOpenGLRenderer::LineVertex QOpenGLRenderer::lineVertexFromScreenPoint(float x, float y) const
{
    return LineVertex{
        (static_cast<GLfloat>(x) / this->r_width) * 2.0f - 1.0f,
        (static_cast<GLfloat>(y) / this->r_height) * 2.0f - 1.0f
    };
}

bool QOpenGLRenderer::drawGLEllipse(int x, int y, int width, int height, const QColor &color, int lineWidth)
{
    if (!this->initializeGLResources() || !this->lineProgram || width <= 0 || height <= 0)
        return false;

    this->flushCommands();
    this->endPainter();

    QVector<LineVertex> vertices;
    const int segments = 72;
    vertices.reserve((segments + 1) * 2);

    float centerX = x + (width / 2.0f);
    float centerY = y + (height / 2.0f);
    float outerRadiusX = width / 2.0f;
    float outerRadiusY = height / 2.0f;
    float innerRadiusX = std::max(0.0f, outerRadiusX - std::max(1, lineWidth));
    float innerRadiusY = std::max(0.0f, outerRadiusY - std::max(1, lineWidth));

    for (int i = 0; i <= segments; ++i)
    {
        float angle = (static_cast<float>(i) / segments) * 2.0f * static_cast<float>(M_PI);
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);
        vertices.append(this->lineVertexFromScreenPoint(centerX + (cosAngle * outerRadiusX), centerY + (sinAngle * outerRadiusY)));
        vertices.append(this->lineVertexFromScreenPoint(centerX + (cosAngle * innerRadiusX), centerY + (sinAngle * innerRadiusY)));
    }

    QOpenGLFunctions *f = this->context->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->lineProgram->bind();
    this->lineProgram->setUniformValue("lineColor", QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));

    this->vertexBuffer.bind();
    this->vertexBuffer.allocate(vertices.constData(), vertices.size() * static_cast<int>(sizeof(LineVertex)));

    int positionLocation = this->lineProgram->attributeLocation("position");
    this->lineProgram->enableAttributeArray(positionLocation);
    this->lineProgram->setAttributeBuffer(positionLocation, GL_FLOAT, offsetof(LineVertex, X), 2, sizeof(LineVertex));

    f->glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());

    this->lineProgram->disableAttributeArray(positionLocation);
    this->vertexBuffer.release();
    this->lineProgram->release();
    this->stats.DrawCalls++;
    return true;
}

bool QOpenGLRenderer::drawGLRoundedRect(int x, int y, int width, int height, int radius, int lineWidth, const QColor &color, bool fill)
{
    if (!this->initializeGLResources() || !this->lineProgram || width <= 0 || height <= 0)
        return false;

    QVector<LineVertex> vertices;
    if (fill)
        this->appendRoundedRectFan(x, y, width, height, radius, &vertices);
    else
        this->appendRoundedRectRing(x, y, width, height, radius, std::max(1, lineWidth), &vertices);

    return this->drawColoredGeometry(vertices, fill ? GL_TRIANGLE_FAN : GL_TRIANGLE_STRIP, color);
}

bool QOpenGLRenderer::drawColoredGeometry(const QVector<LineVertex> &vertices, GLenum primitiveMode, const QColor &color)
{
    if (vertices.isEmpty() || !this->initializeGLResources() || !this->lineProgram)
        return false;

    this->flushCommands();
    this->endPainter();

    QOpenGLFunctions *f = this->context->functions();
    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    this->lineProgram->bind();
    this->lineProgram->setUniformValue("lineColor", QVector4D(color.redF(), color.greenF(), color.blueF(), color.alphaF()));

    this->vertexBuffer.bind();
    this->vertexBuffer.allocate(vertices.constData(), vertices.size() * static_cast<int>(sizeof(LineVertex)));

    int positionLocation = this->lineProgram->attributeLocation("position");
    this->lineProgram->enableAttributeArray(positionLocation);
    this->lineProgram->setAttributeBuffer(positionLocation, GL_FLOAT, offsetof(LineVertex, X), 2, sizeof(LineVertex));

    f->glDrawArrays(primitiveMode, 0, vertices.size());

    this->lineProgram->disableAttributeArray(positionLocation);
    this->vertexBuffer.release();
    this->lineProgram->release();
    this->stats.DrawCalls++;
    return true;
}

void QOpenGLRenderer::appendRoundedRectRing(int x, int y, int width, int height, int radius, int thickness, QVector<LineVertex> *vertices) const
{
    float halfThickness = std::max(1, thickness) / 2.0f;
    QVector<QPointF> outerPoints = this->roundedRectStrokePoints(x, y, width, height, radius, halfThickness);
    QVector<QPointF> innerPoints = this->roundedRectStrokePoints(x, y, width, height, radius, -halfThickness);

    for (int i = 0; i < outerPoints.size() && i < innerPoints.size(); ++i)
    {
        QPointF point = outerPoints[i];
        QPointF inner = innerPoints[i];
        vertices->append(this->lineVertexFromScreenPoint(point.x(), point.y()));
        vertices->append(this->lineVertexFromScreenPoint(inner.x(), inner.y()));
    }

    if (!outerPoints.isEmpty())
    {
        QPointF point = outerPoints[0];
        QPointF inner = innerPoints[0];
        vertices->append(this->lineVertexFromScreenPoint(point.x(), point.y()));
        vertices->append(this->lineVertexFromScreenPoint(inner.x(), inner.y()));
    }
}

void QOpenGLRenderer::appendRoundedRectFan(int x, int y, int width, int height, int radius, QVector<LineVertex> *vertices) const
{
    QVector<QPointF> points = this->roundedRectPoints(x, y, width, height, radius);
    vertices->append(this->lineVertexFromScreenPoint(x + (width / 2.0f), y + (height / 2.0f)));
    foreach (const QPointF &point, points)
        vertices->append(this->lineVertexFromScreenPoint(point.x(), point.y()));
    if (!points.isEmpty())
        vertices->append(this->lineVertexFromScreenPoint(points[0].x(), points[0].y()));
}

QVector<QPointF> QOpenGLRenderer::roundedRectPoints(float x, float y, float width, float height, float radius) const
{
    return this->roundedRectStrokePoints(x, y, width, height, radius, 0);
}

QVector<QPointF> QOpenGLRenderer::roundedRectStrokePoints(float x, float y, float width, float height, float radius, float offset) const
{
    QVector<QPointF> points;
    float strokeX = x - offset;
    float strokeY = y - offset;
    float strokeWidth = std::max(0.0f, width + (offset * 2.0f));
    float strokeHeight = std::max(0.0f, height + (offset * 2.0f));
    float clampedRadius = std::max(0.0f, std::min(radius + offset, std::min(strokeWidth, strokeHeight) / 2.0f));
    const int segments = 12;

    struct Corner
    {
        float CenterX;
        float CenterY;
        float StartAngle;
        float EndAngle;
    };

    Corner corners[4] = {
        { strokeX + strokeWidth - clampedRadius, strokeY + strokeHeight - clampedRadius, 0.0f, static_cast<float>(M_PI) / 2.0f },
        { strokeX + clampedRadius, strokeY + strokeHeight - clampedRadius, static_cast<float>(M_PI) / 2.0f, static_cast<float>(M_PI) },
        { strokeX + clampedRadius, strokeY + clampedRadius, static_cast<float>(M_PI), static_cast<float>(M_PI) * 1.5f },
        { strokeX + strokeWidth - clampedRadius, strokeY + clampedRadius, static_cast<float>(M_PI) * 1.5f, static_cast<float>(M_PI) * 2.0f }
    };

    if (clampedRadius <= 0)
    {
        points.append(QPointF(strokeX + strokeWidth, strokeY + strokeHeight));
        points.append(QPointF(strokeX, strokeY + strokeHeight));
        points.append(QPointF(strokeX, strokeY));
        points.append(QPointF(strokeX + strokeWidth, strokeY));
        return points;
    }

    for (const Corner &corner : corners)
    {
        for (int i = 0; i <= segments; ++i)
        {
            float t = static_cast<float>(i) / segments;
            float angle = corner.StartAngle + ((corner.EndAngle - corner.StartAngle) * t);
            points.append(QPointF(corner.CenterX + (std::cos(angle) * clampedRadius),
                                  corner.CenterY + (std::sin(angle) * clampedRadius)));
        }
    }
    return points;
}

QPointF QOpenGLRenderer::roundedRectInnerPoint(const QPointF &point, const QPointF &center, float thickness) const
{
    QPointF vector = center - point;
    float length = std::sqrt((vector.x() * vector.x()) + (vector.y() * vector.y()));
    if (length <= 0)
        return point;

    float move = std::min(thickness, length);
    return QPointF(point.x() + ((vector.x() / length) * move),
                   point.y() + ((vector.y() / length) * move));
}

int QOpenGLRenderer::worldToQtY(int y) const
{
    return this->r_height - y;
}
