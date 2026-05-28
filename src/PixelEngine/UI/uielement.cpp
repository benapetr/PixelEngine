//! \file uielement.cpp
//! \brief Implementation of base UI element class.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "uielement.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"

using namespace PE;

UIElement::UIElement()
{

}

UIElement::UIElement(const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : Object(position, parent)
{
    this->Width = width;
    this->Height = height;
}

QString UIElement::GetClassName() const
{
    return "PE::UIElement";
}

void UIElement::Serialize(Serializer *serializer) const
{
    Object::Serialize(serializer);
    serializer->WriteFloat("width", this->Width);
    serializer->WriteFloat("height", this->Height);
    serializer->WriteInteger("cornerRadius", this->CornerRadius);
    serializer->WriteBool("enabled", this->Enabled);
    serializer->WriteBool("visible", this->Visible);
}

void UIElement::Deserialize(Deserializer *deserializer)
{
    Object::Deserialize(deserializer);
    this->Width = deserializer->ReadFloat("width", this->Width);
    this->Height = deserializer->ReadFloat("height", this->Height);
    this->CornerRadius = deserializer->ReadInteger("cornerRadius", this->CornerRadius);
    this->Enabled = deserializer->ReadBool("enabled", this->Enabled);
    this->Visible = deserializer->ReadBool("visible", this->Visible);
}

bool UIElement::ContainsPoint(const Vector &point) const
{
    return point.X >= this->Position.X &&
           point.Y >= this->Position.Y &&
           point.X <= this->Position.X + this->Width &&
           point.Y <= this->Position.Y + this->Height;
}

void UIElement::MousePress(const Vector &point)
{
    this->Focused = this->ContainsPoint(point);
}

void UIElement::MouseRelease(const Vector &point)
{
    (void)point;
}

void UIElement::MouseMove(const Vector &point)
{
    (void)point;
}

void UIElement::KeyPress(int key, const QString &text)
{
    (void)key;
    (void)text;
}

void UIElement::DrawBox(Renderer *renderer, int x, int y, int width, int height, int lineWidth, const QColor &color, bool fill) const
{
    if (this->CornerRadius > 0)
        renderer->DrawRoundedRect(x, y, width, height, this->CornerRadius, lineWidth, color, fill);
    else
        renderer->DrawRect(x, y, width, height, lineWidth, color, fill);
}
