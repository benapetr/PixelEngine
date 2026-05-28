//! \file button.cpp
//! \brief Implementation of UI button.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "button.h"
#include "../camera.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"

using namespace PE;

Button::Button()
{

}

Button::Button(const QString &text, const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : UIElement(position, width, height, parent)
{
    this->Text = text;
}

QString Button::GetClassName() const
{
    return "PE::Button";
}

void Button::Render(Renderer *r, Camera *c)
{
    if (!this->Visible)
        return;

    Vector position = c->ProjectedPosition(this->Position);
    QColor fill = this->BackgroundColor;
    if (!this->Enabled)
        fill = QColor(35, 36, 41);
    else if (this->Pressed)
        fill = this->PressedColor;
    else if (this->Hovered)
        fill = this->HoverColor;

    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, fill, true);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BorderColor);
    r->DrawText(position.X2int() + 8, position.Y2int() + this->FontSize + 6, this->Text, this->TextColor, this->FontSize);
}

void Button::MousePress(const Vector &point)
{
    UIElement::MousePress(point);
    this->Pressed = this->Enabled && this->ContainsPoint(point);
    this->RedrawNeeded = true;
}

void Button::MouseRelease(const Vector &point)
{
    bool clicked = this->Enabled && this->Pressed && this->ContainsPoint(point);
    this->Pressed = false;
    this->RedrawNeeded = true;
    if (clicked && this->OnClick)
        this->OnClick();
}

void Button::MouseMove(const Vector &point)
{
    bool hovered = this->Enabled && this->ContainsPoint(point);
    if (this->Hovered != hovered)
    {
        this->Hovered = hovered;
        this->RedrawNeeded = true;
    }
}

void Button::Serialize(Serializer *serializer) const
{
    UIElement::Serialize(serializer);
    serializer->WriteString("text", this->Text);
    serializer->WriteInteger("fontSize", this->FontSize);
}

void Button::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Text = deserializer->ReadString("text", this->Text);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
}
