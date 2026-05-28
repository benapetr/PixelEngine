//! \file textbox.cpp
//! \brief Implementation of UI text box.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "textbox.h"
#include "../camera.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include <Qt>

using namespace PE;

TextBox::TextBox()
{

}

TextBox::TextBox(const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : UIElement(position, width, height, parent)
{
    this->BackgroundColor = QColor(25, 27, 32);
}

QString TextBox::GetClassName() const
{
    return "PE::TextBox";
}

void TextBox::Render(Renderer *r, Camera *c)
{
    if (!this->Visible)
        return;

    Vector position = c->ProjectedPosition(this->Position);
    QColor border = this->Focused ? QColor(98, 154, 255) : this->BorderColor;
    QString text = this->Text.isEmpty() ? this->Placeholder : this->Text;
    QColor textColor = this->Text.isEmpty() ? QColor(140, 145, 155) : this->TextColor;

    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BackgroundColor, true);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, border);
    r->DrawText(position.X2int() + 7, position.Y2int() + this->FontSize + 6, text, textColor, this->FontSize);

    if (this->Focused && this->Editable)
    {
        int caretX = position.X2int() + 8 + (this->Text.length() * (this->FontSize / 2 + 1));
        r->DrawLine(Vector(caretX, position.Y + 6), Vector(caretX, position.Y + this->Height - 6), 1, this->TextColor);
    }
}

void TextBox::MousePress(const Vector &point)
{
    UIElement::MousePress(point);
    this->RedrawNeeded = true;
}

void TextBox::KeyPress(int key, const QString &text)
{
    if (!this->Focused || !this->Editable)
        return;

    if (key == Qt::Key_Backspace)
    {
        if (!this->Text.isEmpty())
            this->Text.chop(1);
    } else if (key == Qt::Key_Delete)
    {
        this->Text.clear();
    } else if (!text.isEmpty() && text.at(0).isPrint() && this->Text.length() < this->MaxLength)
    {
        this->Text.append(text);
    }
    this->RedrawNeeded = true;
}

void TextBox::Serialize(Serializer *serializer) const
{
    UIElement::Serialize(serializer);
    serializer->WriteString("text", this->Text);
    serializer->WriteString("placeholder", this->Placeholder);
    serializer->WriteInteger("fontSize", this->FontSize);
    serializer->WriteInteger("maxLength", this->MaxLength);
    serializer->WriteBool("editable", this->Editable);
}

void TextBox::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Text = deserializer->ReadString("text", this->Text);
    this->Placeholder = deserializer->ReadString("placeholder", this->Placeholder);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
    this->MaxLength = deserializer->ReadInteger("maxLength", this->MaxLength);
    this->Editable = deserializer->ReadBool("editable", this->Editable);
}
