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

QImageRenderer::QImageRenderer(int width, int height)
{
    this->image = new QImage(width, height, QImage::Format_RGB32);
    this->r_width = width;
    this->r_height = height;
    this->painter = new QPainter(this->image);
    this->Clear();
}

QImageRenderer::~QImageRenderer()
{
    delete this->painter;
    delete this->image;
}

void QImageRenderer::Render()
{
    //delete this->image;
}

QImage *QImageRenderer::GetImage()
{
    return this->image;
}

void QImageRenderer::Clear()
{
    this->image->fill(QColor("white"));
}

QPixmap QImageRenderer::GetPixmap()
{
    this->HasUpdate = false;
    return QPixmap::fromImage(*this->image);
}

void QImageRenderer::DrawPixel(int x, int y, QColor color)
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

void QImageRenderer::DrawRect(int x, int y, int width, int height, int line_width, QColor color)
{
    if (!this->Enabled)
        return;

    this->painter->setPen(color);
    this->painter->drawRect(this->trimX(x), this->worldToQtY(y), width, -1 * height);

    // recursive evil :)
    if (line_width > 1)
        this->DrawRect(x + 1, y + 1, width - 2, height - 2, line_width - 1, color);

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

int QImageRenderer::trimX(int x)
{
    if (x < 0)
        return 0;
    if (x > this->r_width)
        return this->r_width;
    return x;
}

int QImageRenderer::trimY(int y)
{
    if (y < 0)
        return 0;
    if (y > this->r_height)
        return this->r_height;
    return y;
}

int QImageRenderer::worldToQtY(int y)
{
    return (this->r_height-1) - y;
}

