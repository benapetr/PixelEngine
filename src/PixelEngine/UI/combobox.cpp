//! \file combobox.cpp
//! \brief Implementation of UI combo box.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "combobox.h"
#include "../camera.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include <QVariantList>

using namespace PE;

ComboBox::ComboBox()
{

}

ComboBox::ComboBox(const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : UIElement(position, width, height, parent)
{

}

QString ComboBox::GetClassName() const
{
    return "PE::ComboBox";
}

void ComboBox::Render(Renderer *r, Camera *c)
{
    if (!this->Visible)
        return;

    Vector position = c->ProjectedPosition(this->Position);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BackgroundColor, true);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BorderColor);
    r->DrawText(position.X2int() + 7, position.Y2int() + this->FontSize + 6, this->GetSelectedText(), this->TextColor, this->FontSize);
    r->DrawText(position.X2int() + static_cast<int>(this->Width) - 18, position.Y2int() + this->FontSize + 6, this->Expanded ? "^" : "v", this->TextColor, this->FontSize);

    if (!this->Expanded)
        return;

    for (int i = 0; i < this->Items.size(); ++i)
    {
        int y = position.Y2int() - static_cast<int>(this->Height) * (i + 1);
        QColor fill = (i == this->SelectedIndex) ? QColor(58, 74, 104) : QColor(30, 32, 38);
        r->DrawRect(position.X2int(), y, static_cast<int>(this->Width), static_cast<int>(this->Height), 1, fill, true);
        r->DrawRect(position.X2int(), y, static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BorderColor);
        r->DrawText(position.X2int() + 7, y + this->FontSize + 6, this->Items[i], this->TextColor, this->FontSize);
    }
}

bool ComboBox::ContainsPoint(const Vector &point) const
{
    pe_float_t totalHeight = this->Height;
    if (this->Expanded)
        totalHeight += this->Height * this->Items.size();

    return point.X >= this->Position.X &&
           point.Y >= this->Position.Y - (totalHeight - this->Height) &&
           point.X <= this->Position.X + this->Width &&
           point.Y <= this->Position.Y + this->Height;
}

void ComboBox::MousePress(const Vector &point)
{
    if (!this->Enabled || !this->ContainsPoint(point))
    {
        this->Expanded = false;
        this->Focused = false;
        this->RedrawNeeded = true;
        return;
    }

    this->Focused = true;
    if (!this->Expanded || point.Y >= this->Position.Y)
    {
        this->Expanded = !this->Expanded;
        this->RedrawNeeded = true;
        return;
    }

    int index = static_cast<int>((this->Position.Y - point.Y) / this->Height);
    if (index >= 0 && index < this->Items.size())
    {
        this->SelectedIndex = index;
        if (this->OnSelectionChanged)
            this->OnSelectionChanged(index, this->Items[index]);
    }
    this->Expanded = false;
    this->RedrawNeeded = true;
}

void ComboBox::Serialize(Serializer *serializer) const
{
    UIElement::Serialize(serializer);
    QVariantList items;
    foreach (const QString &item, this->Items)
        items.append(item);
    serializer->WriteValue("items", items);
    serializer->WriteInteger("selectedIndex", this->SelectedIndex);
    serializer->WriteInteger("fontSize", this->FontSize);
}

void ComboBox::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Items.clear();
    QVariantList items = deserializer->ReadValue("items").toList();
    foreach (const QVariant &item, items)
        this->Items.append(item.toString());
    this->SelectedIndex = deserializer->ReadInteger("selectedIndex", this->SelectedIndex);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
}

void ComboBox::AddItem(const QString &item)
{
    this->Items.append(item);
    if (this->SelectedIndex < 0)
        this->SelectedIndex = 0;
    this->RedrawNeeded = true;
}

void ComboBox::ClearItems()
{
    this->Items.clear();
    this->SelectedIndex = -1;
    this->RedrawNeeded = true;
}

QString ComboBox::GetSelectedText() const
{
    if (this->SelectedIndex < 0 || this->SelectedIndex >= this->Items.size())
        return QString();
    return this->Items[this->SelectedIndex];
}
