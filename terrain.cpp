// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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

    // Check painter mode
    if (this->painterMode != PainterMode_Destructing)
        this->changePainterMode(PainterMode_Destructing);

    this->getPainter()->drawPoint(x, this->terrainToQtY(y));
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

    this->getPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = true;
}

void Terrain::DestroyPixelAbsolute(int x, int y)
{
    // Check painter mode
    if (this->painterMode != PainterMode_Destructing)
        this->changePainterMode(PainterMode_Destructing);

    this->getPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = false;
}

void Terrain::CreatePixelAbsolute(int x, int y)
{
    // Check painter mode
    if (this->painterMode != PainterMode_Creating)
        this->changePainterMode(PainterMode_Creating);

    this->getPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = true;
}

void Terrain::RefreshPixmap()
{
    this->BitMap = QPixmap::fromImage(this->SourceImage);
    this->LastMovementUpdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

int Terrain::ShiftFloatingBitsDownByOnePixel()
{
    int shifted_pixels = 0;
    int current_column = 0;
    while (current_column < this->t_width)
    {
        if (!this->Collider->Bitmap[current_column][0])
            this->CreatePixelAbsolute(current_column, 0);
        int current_pixel = 0;
        while (++current_pixel < this->t_height)
        {
            if (!this->Collider->Bitmap[current_column][current_pixel])
            {
                // This pixel is air, let's check if there is anything above it that isn't air
                // if that's the case we need to shift floating column down
                while (++current_pixel < this->t_height)
                {
                    if (this->Collider->Bitmap[current_column][current_pixel])
                        break;
                }
                if (current_pixel < this->t_height)
                {
                    // This pixel is air, so let's shift everything in this column down one pixel
                    while (current_pixel < this->t_height)
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
    return shifted_pixels;
}

int Terrain::ShiftFloatingBitsDown()
{
    int shifted_pixels = 0;
    int current_column = 0;
    while (current_column < this->t_width)
    {
        if (!this->Collider->Bitmap[current_column][0])
            this->CreatePixelAbsolute(current_column, 0);
        int current_pixel = 0;
        while (++current_pixel < this->t_height)
        {
            if (!this->Collider->Bitmap[current_column][current_pixel])
            {
                int hole_size = 0;
                // This pixel is air, let's check if there is anything above it that isn't air
                // if that's the case we need to shift floating column down
                while (++current_pixel < this->t_height)
                {
                    hole_size++;
                    if (this->Collider->Bitmap[current_column][current_pixel])
                        break;
                }
                if (current_pixel < this->t_height)
                {
                    // This pixel is air, so let's shift everything in this column down one pixel
                    while (current_pixel < this->t_height)
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
    return shifted_pixels;
}

int Terrain::terrainToQtY(int y)
{
    return (this->t_height) - y;
}

QPainter *Terrain::getPainter()
{
    if (!this->painter)
    {
        this->painter = new QPainter(&this->SourceImage);
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
            this->painter->setPen(this->BackgroundColor);
            break;
        case PainterMode_Creating:
            this->painter->setPen(this->TerrainColor);
            break;
    }
}
