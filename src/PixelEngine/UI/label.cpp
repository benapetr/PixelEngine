//! \file label.cpp
//! \brief Implementation of UI label.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "label.h"
#include "../camera.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"

using namespace PE;

Label::Label()
{

}

Label::Label(const QString &text, const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : UIElement(position, width, height, parent)
{
    this->Text = text;
}

QString Label::GetClassName() const
{
    return "PE::Label";
}

void Label::Render(Renderer *r, Camera *c)
{
    if (!this->Visible)
        return;

    Vector position = c->ProjectedPosition(this->Position);
    if (this->DrawBackground)
        r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BackgroundColor, true);
    r->DrawText(position.X2int() + 6, position.Y2int() + this->FontSize + 3, this->Text, this->TextColor, this->FontSize);
}

void Label::Serialize(Serializer *serializer) const
{
    UIElement::Serialize(serializer);
    serializer->WriteString("text", this->Text);
    serializer->WriteInteger("fontSize", this->FontSize);
    serializer->WriteBool("drawBackground", this->DrawBackground);
}

void Label::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Text = deserializer->ReadString("text", this->Text);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
    this->DrawBackground = deserializer->ReadBool("drawBackground", this->DrawBackground);
}
