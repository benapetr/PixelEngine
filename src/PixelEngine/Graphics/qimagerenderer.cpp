//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "qimagerenderer.h"
#include <QPainter>
#include <QImage>

using namespace PE;

QImageRenderer::QImageRenderer(int width, int height) : Renderer(width, height)
{
    this->image = new QImage(width, height, QImage::Format_RGB32);
    this->painter = new QPainter(this->image);
    this->Clear();
}

QImageRenderer::~QImageRenderer()
{
    delete this->painter;
    delete this->image;
}

QImage *QImageRenderer::GetImage()
{
    return this->image;
}

RendererType QImageRenderer::GetType()
{
    return RendererType_QImage;
}

void QImageRenderer::Clear()
{
    this->image->fill(Qt::white);
}

void QImageRenderer::Clear(const QColor &color)
{
    this->image->fill(color);
}

QPixmap QImageRenderer::GetPixmap()
{
    this->HasUpdate = false;
    return QPixmap::fromImage(*this->image);
}

void QImageRenderer::DrawPixel(int x, int y, const QColor &color)
{
    if (!this->Enabled)
        return;
    if (x < 0 || x > this->r_width)
        return;
    if (y < 0 || y > this->r_height)
        return;
    this->image->setPixel(x, this->worldToQtY(y), color.rgb());
    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QImageRenderer::DrawBitmap(int x, int y, int width, int height, const QPixmap &bitmap)
{
    if (!this->Enabled)
        return;
    this->painter->setPen(QPen());
    this->painter->drawPixmap(x, this->worldToQtY(y + height), width, height, bitmap);
    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QImageRenderer::DrawLine(Vector source, Vector target, int line_width, const QColor &color)
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

void QImageRenderer::DrawRect(int x, int y, int width, int height, int line_width, const QColor &color, bool fill)
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

void QImageRenderer::DrawText(int x, int y, const QString &text, const QColor &color, int size)
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

void QImageRenderer::DrawEllipse(int x, int y, int width, int height, const QColor &color, int line_width)
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

int QImageRenderer::worldToQtY(int y)
{
    return (this->r_height) - y;
}

