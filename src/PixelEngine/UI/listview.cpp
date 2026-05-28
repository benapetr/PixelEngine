//! \file listview.cpp
//! \brief Implementation of UI list view.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "listview.h"
#include "../camera.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include <QVariantList>
#include <Qt>
#include <algorithm>

using namespace PE;

ListView::ListView()
{

}

ListView::ListView(const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : UIElement(position, width, height, parent)
{
    this->BackgroundColor = QColor(25, 27, 32);
}

QString ListView::GetClassName() const
{
    return "PE::ListView";
}

void ListView::Render(Renderer *r, Camera *c)
{
    if (!this->Visible)
        return;

    this->VerticalScroll.SetSizes(this->Height, this->Items.size() * this->ItemHeight);

    Vector position = c->ProjectedPosition(this->Position);
    int x = position.X2int();
    int y = position.Y2int();
    int width = static_cast<int>(this->Width);
    int height = static_cast<int>(this->Height);
    int contentWidth = width - ((this->ShowScrollBar && this->VerticalScroll.CanScroll()) ? this->ScrollBarWidth : 0);

    this->DrawBox(r, x, y, width, height, 1, this->BackgroundColor, true);
    this->DrawBox(r, x, y, width, height, 1, this->BorderColor);

    r->PushClipRect(x + 1, y + 1, contentWidth - 2, height - 2);
    int firstIndex = static_cast<int>(this->VerticalScroll.GetOffset() / this->ItemHeight);
    int hiddenPixels = static_cast<int>(this->VerticalScroll.GetOffset()) % this->ItemHeight;
    int visibleCount = static_cast<int>(this->Height / this->ItemHeight) + 2;

    for (int row = 0; row < visibleCount; ++row)
    {
        int index = firstIndex + row;
        if (index < 0 || index >= this->Items.size())
            continue;

        int itemTop = y + height - ((row + 1) * this->ItemHeight) + hiddenPixels;
        if (itemTop + this->ItemHeight < y || itemTop > y + height)
            continue;

        QColor fill = (index == this->SelectedIndex) ? this->SelectedColor : ((index == this->HoveredIndex) ? this->HoverColor : ((index % 2) ? this->AlternateColor : this->BackgroundColor));
        r->DrawRect(x + 1, itemTop, contentWidth - 2, this->ItemHeight, 1, fill, true);
        r->DrawText(x + 7, itemTop + ((this->ItemHeight - this->FontSize) / 2), this->Items[index], this->TextColor, this->FontSize);
    }
    r->PopClipRect();

    if (this->isScrollBarVisible())
    {
        int trackX = x + width - this->ScrollBarWidth - 1;
        int trackHeight = height - 2;
        r->DrawRect(trackX, y + 1, this->ScrollBarWidth, trackHeight, 1, this->ScrollTrackColor, true);

        int thumbSize = static_cast<int>(this->VerticalScroll.GetThumbSize(trackHeight));
        int thumbOffset = static_cast<int>(this->VerticalScroll.GetThumbOffset(trackHeight));
        int thumbY = y + height - 1 - thumbOffset - thumbSize;
        r->DrawRect(trackX + 1, thumbY, this->ScrollBarWidth - 2, thumbSize, 1, this->ScrollThumbColor, true);
    }
}

void ListView::MousePress(const Vector &point)
{
    UIElement::MousePress(point);
    if (!this->Focused)
        return;

    this->VerticalScroll.SetSizes(this->Height, this->Items.size() * this->ItemHeight);
    if (this->isScrollBarVisible() && this->isPointOnScrollTrack(point))
    {
        pe_float_t trackSize = this->Height - 2;
        pe_float_t thumbSize = this->VerticalScroll.GetThumbSize(trackSize);
        pe_float_t thumbOffset = this->VerticalScroll.GetThumbOffset(trackSize);
        pe_float_t thumbBottom = this->Position.Y + this->Height - 1 - thumbOffset - thumbSize;
        pe_float_t thumbTop = thumbBottom + thumbSize;

        if (this->isPointOnScrollThumb(point))
        {
            this->IsDraggingScrollBar = true;
            this->ScrollDragOffset = thumbTop - point.Y;
        }
        else
        {
            this->VerticalScroll.ScrollPage(point.Y > thumbTop ? -1 : 1);
        }
        this->RedrawNeeded = true;
        return;
    }

    pe_float_t localYFromTop = (this->Position.Y + this->Height) - point.Y;
    int index = static_cast<int>((localYFromTop + this->VerticalScroll.GetOffset()) / this->ItemHeight);
    if (index >= 0 && index < this->Items.size())
    {
        this->SelectedIndex = index;
        if (this->OnSelectionChanged)
            this->OnSelectionChanged(index, this->Items[index]);
    }
    this->RedrawNeeded = true;
}

void ListView::MouseRelease(const Vector &point)
{
    (void)point;
    this->IsDraggingScrollBar = false;
}

void ListView::MouseMove(const Vector &point)
{
    if (this->IsDraggingScrollBar)
    {
        this->setScrollFromThumbTop(point.Y + this->ScrollDragOffset);
        this->RedrawNeeded = true;
        return;
    }

    int previousIndex = this->HoveredIndex;
    this->HoveredIndex = -1;

    if (this->ContainsPoint(point))
    {
        this->VerticalScroll.SetSizes(this->Height, this->Items.size() * this->ItemHeight);
        pe_float_t localYFromTop = (this->Position.Y + this->Height) - point.Y;
        int index = static_cast<int>((localYFromTop + this->VerticalScroll.GetOffset()) / this->ItemHeight);
        if (index >= 0 && index < this->Items.size())
            this->HoveredIndex = index;
    }

    if (this->HoveredIndex != previousIndex)
        this->RedrawNeeded = true;
}

bool ListView::MouseWheel(const Vector &point, pe_float_t delta)
{
    if (!this->Enabled || !this->Visible || !this->ContainsPoint(point))
        return false;

    this->VerticalScroll.SetSizes(this->Height, this->Items.size() * this->ItemHeight);
    if (!this->VerticalScroll.CanScroll())
        return false;

    pe_float_t previousOffset = this->VerticalScroll.GetOffset();
    this->VerticalScroll.ScrollBy(-delta);
    this->RedrawNeeded = previousOffset != this->VerticalScroll.GetOffset();
    return true;
}

void ListView::KeyPress(int key, const QString &text)
{
    (void)text;
    if (!this->Focused)
        return;

    if (key == Qt::Key_Down)
        this->VerticalScroll.ScrollLine(1);
    else if (key == Qt::Key_Up)
        this->VerticalScroll.ScrollLine(-1);
    else if (key == Qt::Key_PageDown)
        this->VerticalScroll.ScrollPage(1);
    else if (key == Qt::Key_PageUp)
        this->VerticalScroll.ScrollPage(-1);

    this->RedrawNeeded = true;
}

void ListView::Serialize(Serializer *serializer) const
{
    UIElement::Serialize(serializer);
    QVariantList items;
    foreach (const QString &item, this->Items)
        items.append(item);
    serializer->WriteValue("items", items);
    serializer->WriteInteger("selectedIndex", this->SelectedIndex);
    serializer->WriteInteger("itemHeight", this->ItemHeight);
    serializer->WriteInteger("fontSize", this->FontSize);
    serializer->WriteBool("showScrollBar", this->ShowScrollBar);
}

void ListView::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Items.clear();
    QVariantList items = deserializer->ReadValue("items").toList();
    foreach (const QVariant &item, items)
        this->Items.append(item.toString());
    this->SelectedIndex = deserializer->ReadInteger("selectedIndex", this->SelectedIndex);
    this->ItemHeight = deserializer->ReadInteger("itemHeight", this->ItemHeight);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
    this->ShowScrollBar = deserializer->ReadBool("showScrollBar", this->ShowScrollBar);
}

void ListView::AddItem(const QString &item)
{
    this->Items.append(item);
    this->VerticalScroll.SetSizes(this->Height, this->Items.size() * this->ItemHeight);
    this->RedrawNeeded = true;
}

void ListView::ClearItems()
{
    this->Items.clear();
    this->SelectedIndex = -1;
    this->VerticalScroll.SetOffset(0);
    this->RedrawNeeded = true;
}

QString ListView::GetSelectedText() const
{
    if (this->SelectedIndex < 0 || this->SelectedIndex >= this->Items.size())
        return QString();
    return this->Items[this->SelectedIndex];
}

bool ListView::isScrollBarVisible() const
{
    return this->ShowScrollBar && this->VerticalScroll.CanScroll() && this->ScrollBarWidth > 0;
}

bool ListView::isPointOnScrollTrack(const Vector &point) const
{
    if (!this->isScrollBarVisible())
        return false;

    pe_float_t trackX = this->Position.X + this->Width - this->ScrollBarWidth - 1;
    return point.X >= trackX &&
           point.X <= trackX + this->ScrollBarWidth &&
           point.Y >= this->Position.Y + 1 &&
           point.Y <= this->Position.Y + this->Height - 1;
}

bool ListView::isPointOnScrollThumb(const Vector &point) const
{
    if (!this->isPointOnScrollTrack(point))
        return false;

    pe_float_t trackSize = this->Height - 2;
    pe_float_t thumbSize = this->VerticalScroll.GetThumbSize(trackSize);
    pe_float_t thumbOffset = this->VerticalScroll.GetThumbOffset(trackSize);
    pe_float_t thumbBottom = this->Position.Y + this->Height - 1 - thumbOffset - thumbSize;
    pe_float_t thumbTop = thumbBottom + thumbSize;

    return point.Y >= thumbBottom && point.Y <= thumbTop;
}

void ListView::setScrollFromThumbTop(pe_float_t thumbTopFromWindowBottom)
{
    pe_float_t trackSize = this->Height - 2;
    pe_float_t thumbSize = this->VerticalScroll.GetThumbSize(trackSize);
    pe_float_t trackTravel = std::max<pe_float_t>(1, trackSize - thumbSize);
    pe_float_t topLimit = this->Position.Y + this->Height - 1;
    pe_float_t thumbOffset = topLimit - thumbTopFromWindowBottom;
    pe_float_t ratio = std::max<pe_float_t>(0, std::min<pe_float_t>(1, thumbOffset / trackTravel));
    this->VerticalScroll.SetOffset(this->VerticalScroll.GetMaxOffset() * ratio);
}
