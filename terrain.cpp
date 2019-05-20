// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "terrain.h"
#include "camera.h"
#include "bitmapcollider.h"
#include "renderer.h"

using namespace PE;

Terrain::Terrain(double x, double y, int width, int height)
{
    this->Position = Vector(x, y);
    this->Collider = new BitmapCollider(static_cast<int>(x), static_cast<int>(y), width, height);
}

Terrain::~Terrain()
{
    //delete this->Collider;
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
