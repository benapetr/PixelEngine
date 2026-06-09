//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "sprite.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include "../camera.h"
#include "../resources.h"

using namespace PE;

Sprite::Sprite()
{

}

Sprite::Sprite(const Vector &position, int width, int height, const QString &resourceName, Object *parent) : Object(position, parent)
{
    this->Width = width;
    this->Height = height;
    this->ResourceName = resourceName;
}

QString Sprite::GetClassName() const
{
    return "PE::Sprite";
}

void Sprite::Serialize(Serializer *serializer) const
{
    Object::Serialize(serializer);
    serializer->WriteString("resourceName", this->ResourceName);
    serializer->WriteInteger("width", this->Width);
    serializer->WriteInteger("height", this->Height);
}

void Sprite::Deserialize(Deserializer *deserializer)
{
    Object::Deserialize(deserializer);
    this->ResourceName = deserializer->ReadString("resourceName", this->ResourceName);
    this->Width = deserializer->ReadInteger("width", this->Width);
    this->Height = deserializer->ReadInteger("height", this->Height);
}

void Sprite::Render(Renderer *r, Camera *c)
{
    const QPixmap *pixmap = nullptr;
    if (!this->Pixmap.isNull())
        pixmap = &this->Pixmap;
    else if (!this->ResourceName.isEmpty())
        pixmap = &Resources::GetPixmap(this->ResourceName);

    if (!pixmap || pixmap->isNull())
        return;

    int width = this->Width > 0 ? this->Width : pixmap->width();
    int height = this->Height > 0 ? this->Height : pixmap->height();
    Vector position = c->ProjectedPosition(this->Position);
    r->DrawBitmapRotated(position.X2int(), position.Y2int(), width, height, *pixmap, this->GetWorldRotation());
}

void Sprite::SetPixmap(const QPixmap &pixmap)
{
    this->Pixmap = pixmap;
    if (this->Width <= 0)
        this->Width = pixmap.width();
    if (this->Height <= 0)
        this->Height = pixmap.height();
    this->RedrawNeeded = true;
}

void Sprite::SetResourceName(const QString &resourceName)
{
    this->ResourceName = resourceName;
    this->RedrawNeeded = true;
}
