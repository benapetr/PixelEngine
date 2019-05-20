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
