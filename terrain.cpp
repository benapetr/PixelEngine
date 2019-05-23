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
    this->getPainter()->drawPoint(x, this->terrainToQtY(y));
    this->Collider->Bitmap[x][y] = false;
}

void Terrain::RefreshPixmap()
{
    this->BitMap = QPixmap::fromImage(this->SourceImage);
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
