//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "terrain.h"
#include "camera.h"
#include "Physics/bitmapcollider.h"
#include "Graphics/renderer.h"
#include <QDateTime>
#include <QPainter>

using namespace PE;

Terrain::Terrain(double x, double y, int width, int height)
{
    this->Position = Vector(x, y);
    this->Collider = new BitmapCollider(static_cast<int>(x), static_cast<int>(y), width, height, this);
    this->t_width = width;
    this->t_height = height;
}

Terrain::~Terrain()
{
    delete this->painter;
}

PE_ObjectType Terrain::GetType()
{
    return PE_ObjectType_Terrain;
}

void Terrain::Render(Renderer *r, Camera *c)
{
    Vector position = c->ProjectedPosition(this->Position);
    r->DrawBitmap(position.X2int(), position.Y2int(), this->Collider->GetWidth(), this->Collider->GetHeight(), this->BitMap);
}

void Terrain::SetPosition(Vector p)
{
    Object::SetPosition(p);
    this->Collider->Position = p;
}

void Terrain::DestroyPixel(Vector p)
{
    int x = static_cast<int>(p.X);
    int y = static_cast<int>(p.Y);
    this->DestroyPixel(x, y);
}

void Terrain::DestroyPixel(int x, int y)
{
    x -= this->Position.X2int();
    y -= this->Position.Y2int();
    if (x < 0 || x >= this->t_width)
        return;
    if (y < 0 || y >= this->t_height)
        return;

    if (this->mostLeftModPix < 0 || x < this->mostLeftModPix)
        this->mostLeftModPix = x;
    if (x > this->mostRightModPix)
        this->mostRightModPix = x;
    if (this->mostDownModPix < 0 || y < this->mostDownModPix)
        this->mostDownModPix = y;
    if (this->mostTopModPix < y)
        this->mostTopModPix = y;

    // Check painter mode
    if (this->painterMode != PainterMode_Destructing)
        this->changePainterMode(PainterMode_Destructing);

    this->GetPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = false;
}

void Terrain::CreatePixel(int x, int y)
{
    x -= this->Position.X2int();
    y -= this->Position.Y2int();
    if (x < 0 || x >= this->t_width)
        return;
    if (y < 0 || y >= this->t_height)
        return;

    // Check painter mode
    if (this->painterMode != PainterMode_Creating)
        this->changePainterMode(PainterMode_Creating);

    this->GetPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = true;
}

void Terrain::DestroyPixelAbsolute(int x, int y)
{
    // Check painter mode
    if (this->painterMode != PainterMode_Destructing)
        this->changePainterMode(PainterMode_Destructing);

    this->GetPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = false;
}

void Terrain::CreatePixelAbsolute(int x, int y)
{
    // Check painter mode
    if (this->painterMode != PainterMode_Creating)
        this->changePainterMode(PainterMode_Creating);

    this->GetPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = true;
}

void Terrain::DrawLine(int x1, int y1, int x2, int y2, const QColor& color)
{
    x1 -= this->Position.X2int();
    y1 -= this->Position.Y2int();
    if (x1 < 0 || x1 >= this->t_width)
        return;
    if (y1 < 0 || y1 >= this->t_height)
        return;
    x2 -= this->Position.X2int();
    y2 -= this->Position.Y2int();
    if (x2 < 0 || x2 >= this->t_width)
        return;
    if (y2 < 0 || y2 >= this->t_height)
        return;
    this->GetPainter()->setPen(color);
    this->GetPainter()->drawLine(x1, this->terrainToQtY(y1), x2, this->terrainToQtY(y2));
}

void Terrain::DrawPixel(int x, int y, const QColor& color)
{
    x -= this->Position.X2int();
    y -= this->Position.Y2int();
    if (x < 0 || x >= this->t_width)
        return;
    if (y < 0 || y >= this->t_height)
        return;

    this->GetPainter()->setPen(color);
    this->GetPainter()->drawPoint(x, this->terrainToQtY(y));
}

void Terrain::DrawBitmap(int x, int y, int width, int height, QPixmap bitmap)
{
    this->GetPainter()->drawPixmap(x, this->terrainToQtY(y + height), width, height, bitmap);
}

void Terrain::RefreshPixmap()
{
    this->BitMap = QPixmap::fromImage(this->sourceImage);
    this->LastMovementUpdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

int Terrain::ShiftFloatingBitsDownByOnePixel()
{
    int shifted_pixels = 0;
    int current_column = 0;

    ///////////////////////
    // Performance hacks //
    ///////////////////////

    if (this->mostLeftModPix > -1)
        current_column = this->mostLeftModPix;

    int max_col, max_row, min_row;
    if (this->mostRightModPix > -1)
        max_col = this->mostRightModPix;
    else
        max_col = this->t_width;

    if (this->mostDownModPix > -1)
        min_row = this->mostDownModPix;
    else
        min_row = 0;

    if (this->mostTopModPix > -1)
        max_row = this->mostTopModPix;
    else
        max_row = this->t_height;

    ///////////////////////
    ///////////////////////

    while (current_column < max_col)
    {
        if (!this->Collider->Bitmap[current_column][0])
            this->CreatePixelAbsolute(current_column, 0);
        int current_pixel = min_row;
        while (++current_pixel < max_row)
        {
            if (!this->Collider->Bitmap[current_column][current_pixel])
            {
                // This pixel is air, let's check if there is anything above it that isn't air
                // if that's the case we need to shift floating column down
                while (++current_pixel < max_row)
                {
                    if (this->Collider->Bitmap[current_column][current_pixel])
                        break;
                }
                if (current_pixel < max_row)
                {
                    // This pixel is air, so let's shift everything in this column down one pixel
                    while (current_pixel < max_row)
                    {
                        shifted_pixels++;
                        bool is_space = !this->Collider->Bitmap[current_column][current_pixel];
                        //! \todo This can be done for whole slope
                        if (!is_space)
                        {
                            //this->DestroyPixel(current_column, current_pixel);
                            if (!this->Collider->Bitmap[current_column][current_pixel - 1])
                                this->CreatePixelAbsolute(current_column, current_pixel - 1);
                        } else
                        {
                            //this->DestroyPixel(current_column, current_pixel);
                            if (this->Collider->Bitmap[current_column][current_pixel - 1])
                                this->DestroyPixelAbsolute(current_column, current_pixel - 1);
                        }
                        current_pixel++;
                    }
                }
            }
        }
        // 0 pixel is always ground
        if (!this->Collider->Bitmap[current_column][0])
            this->CreatePixelAbsolute(current_column, 0);
        current_column++;
    }
    this->mostTopModPix = -1;
    this->mostDownModPix = -1;
    this->mostLeftModPix = -1;
    this->mostRightModPix = -1;
    return shifted_pixels;
}

int Terrain::ShiftFloatingBitsDown()
{
    int shifted_pixels = 0;
    int current_column = 0;

    ///////////////////////
    // Performance hacks //
    ///////////////////////

    if (this->mostLeftModPix > -1)
        current_column = this->mostLeftModPix;

    int max_col, max_row, min_row;
    if (this->mostRightModPix > -1)
        max_col = this->mostRightModPix;
    else
        max_col = this->t_width;

    if (this->mostDownModPix > -1)
        min_row = this->mostDownModPix;
    else
        min_row = 0;

    if (this->mostTopModPix > -1)
        max_row = this->mostTopModPix;
    else
        max_row = this->t_height;

    ///////////////////////
    ///////////////////////

    while (current_column < max_col)
    {
        if (!this->Collider->Bitmap[current_column][0])
            this->CreatePixelAbsolute(current_column, 0);
        int current_pixel = 0;
        while (++current_pixel < max_row)
        {
            if (!this->Collider->Bitmap[current_column][current_pixel])
            {
                int hole_size = 0;
                // This pixel is air, let's check if there is anything above it that isn't air
                // if that's the case we need to shift floating column down
                while (++current_pixel < max_row)
                {
                    hole_size++;
                    if (this->Collider->Bitmap[current_column][current_pixel])
                        break;
                }
                if (current_pixel < max_row)
                {
                    // This pixel is air, so let's shift everything in this column down one pixel
                    while (current_pixel < max_row)
                    {
                        shifted_pixels++;
                        bool is_space = !this->Collider->Bitmap[current_column][current_pixel];
                        //! \todo This can be done for whole slope
                        if (!is_space)
                        {
                            //this->DestroyPixel(current_column, current_pixel);
                            if (!this->Collider->Bitmap[current_column][current_pixel - hole_size])
                                this->CreatePixelAbsolute(current_column, current_pixel - hole_size);
                        } else
                        {
                            //this->DestroyPixel(current_column, current_pixel);
                            if (this->Collider->Bitmap[current_column][current_pixel - hole_size])
                                this->DestroyPixelAbsolute(current_column, current_pixel - hole_size);
                        }
                        current_pixel++;
                    }
                }
            }
        }
        // 0 pixel is always ground
        if (!this->Collider->Bitmap[current_column][0])
            this->CreatePixelAbsolute(current_column, 0);
        current_column++;
    }
    this->mostTopModPix = -1;
    this->mostDownModPix = -1;
    this->mostLeftModPix = -1;
    this->mostRightModPix = -1;
    return shifted_pixels;
}

QImage Terrain::GetSourceImage()
{
    return this->sourceImage;
}

void Terrain::SetSourceImage(const QImage &image)
{
    delete this->painter;
    this->painter = nullptr;
    this->sourceImage = image;
}

int Terrain::terrainToQtY(int y)
{
    return (this->t_height) - y;
}

QPainter *Terrain::GetPainter()
{
    if (!this->painter)
    {
        this->painter = new QPainter(&this->sourceImage);
        this->painter->setPen(this->BackgroundColor);
        //this->painter->setPen(QColor("orange"));
    }
    return this->painter;
}

void Terrain::changePainterMode(Terrain::PainterMode mode)
{
    this->painterMode = mode;
    switch(mode)
    {
        case PainterMode_Destructing:
            this->GetPainter()->setPen(this->BackgroundColor);
            break;
        case PainterMode_Creating:
        case PainterMode_CreatingSpecial:
            this->GetPainter()->setPen(this->TerrainColor);
            break;
    }
}
