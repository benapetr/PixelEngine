// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
    this->image->fill(Qt::white);
}

void QImageRenderer::Clear(QColor color)
{
    this->image->fill(color);
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

void QImageRenderer::DrawBitmap(int x, int y, int width, int height, QBitmap bitmap)
{
    if (!this->Enabled)
        return;
    this->painter->setPen(QPen());
    this->painter->drawPixmap(x, this->worldToQtY(y) - height, width, height, bitmap);
    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QImageRenderer::DrawLine(Vector source, Vector target, int line_width, QColor color)
{
    if (!this->Enabled)
        return;

    QPen pen(color);
    pen.setWidth(line_width);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::MiterJoin);
    this->painter->setPen(pen);
    this->painter->drawLine(this->trimX(source.X2int()), this->worldToQtY(source.Y2int()), this->trimX(target.X2int()), this->worldToQtY(target.Y2int()));

    if (!this->ManualUpdate)
        this->HasUpdate = true;
}

void QImageRenderer::DrawRect(int x, int y, int width, int height, int line_width, QColor color, bool fill)
{
    if (!this->Enabled)
        return;

    if (fill)
    {
        QBrush brush(color, Qt::SolidPattern);
        this->painter->fillRect(this->trimX(x), this->worldToQtY(y + height), width, height, brush);
    }
    else
    {
        QPen pen(color);
        pen.setWidth(line_width);
        pen.setCapStyle(Qt::SquareCap);
        pen.setJoinStyle(Qt::MiterJoin);
        this->painter->setPen(pen);
        this->painter->drawRect(this->trimX(x), this->worldToQtY(y + height), width, height);
    }

    // recursive evil :)
    //if (line_width > 1)
    //    this->DrawRect(x + 1, y + 1, width - 2, height - 2, line_width - 1, color);

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

