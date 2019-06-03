//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "qglrenderer.h"
#include <QPainter>
#include <QOpenGLWidget>
#include <QImage>

using namespace PE;

QGLRenderer::QGLRenderer(int width, int height, QOpenGLWidget *widget) : Renderer(width, height)
{
    this->owner = widget;
}

QGLRenderer::~QGLRenderer()
{
    delete this->painter;
}

RendererType QGLRenderer::GetType()
{
    return RendererType_OpenGL;
}

void QGLRenderer::Clear()
{
    this->painter->fillRect(0, 0, this->r_width, this->r_height, Qt::white);
}

void QGLRenderer::Clear(const QColor &color)
{
    this->painter->fillRect(0, 0, this->r_width, this->r_height, color);
}

void QGLRenderer::DrawPixel(int x, int y, const QColor &color)
{
    if (!this->Enabled)
        return;
    if (x < 0 || x > this->r_width)
        return;
    if (y < 0 || y > this->r_height)
        return;
    QPen pen(color);
    this->painter->setPen(pen);
    this->painter->drawPoint(x, y);
    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QGLRenderer::DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap)
{
    if (!this->Enabled)
        return;
    this->painter->setPen(QPen());
    this->painter->drawPixmap(x, this->worldToQtY(y + height), width, height, bitmap);
    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QGLRenderer::DrawLine(Vector source, Vector target, int line_width, const QColor &color)
{
    if (!this->Enabled)
        return;

    QPen pen(color);
    pen.setWidth(line_width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    this->painter->setPen(pen);
    this->painter->drawLine(source.X2int(), this->worldToQtY(source.Y2int()), target.X2int(), this->worldToQtY(target.Y2int()));

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QGLRenderer::DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill)
{
    if (!this->Enabled)
        return;

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

void QGLRenderer::DrawText(int x, int y, const QString &text, const QColor &color, int size)
{
    if (!this->Enabled)
        return;

    QPen pen(color);
    QFont font;
    font.setPixelSize(size);
    this->painter->setPen(pen);
    this->painter->setFont(font);
    this->painter->drawText(x, this->worldToQtY(y), text);

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QGLRenderer::DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width)
{
    if (!this->Enabled)
        return;

    QPen pen(color);
    pen.setWidth(line_width);
    this->painter->setPen(pen);
    this->painter->drawEllipse(x, this->worldToQtY(y + height), width, height);

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QGLRenderer::Begin()
{
    if (!this->painter)
        this->painter = new QPainter();
    this->painter->begin(this->owner);
}

void QGLRenderer::End()
{
    this->painter->end();
}

int QGLRenderer::worldToQtY(int y)
{
    return (this->r_height) - y;
}

