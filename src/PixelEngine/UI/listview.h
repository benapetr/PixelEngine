//! \file listview.h
//! \brief Header file for UI list view.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "scrollstate.h"
#include "uielement.h"
#include <QStringList>
#include <functional>

namespace PE
{
    class ListView : public UIElement
    {
        public:
            ListView();
            ListView(const Vector &position, pe_float_t width, pe_float_t height, Object *parent = nullptr);
            QString GetClassName() const override;
            void Render(Renderer *r, Camera *c) override;
            void MousePress(const Vector &point) override;
            void MouseRelease(const Vector &point) override;
            void MouseMove(const Vector &point) override;
            bool MouseWheel(const Vector &point, pe_float_t delta) override;
            void KeyPress(int key, const QString &text = QString()) override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            void AddItem(const QString &item);
            void ClearItems();
            QString GetSelectedText() const;

            QStringList Items;
            ScrollState VerticalScroll;
            int SelectedIndex = -1;
            int HoveredIndex = -1;
            int ItemHeight = 24;
            int FontSize = 13;
            int ScrollBarWidth = 10;
            bool ShowScrollBar = true;
            bool IsDraggingScrollBar = false;
            pe_float_t ScrollDragOffset = 0;
            QColor SelectedColor = QColor(58, 74, 104);
            QColor HoverColor = QColor(50, 56, 68);
            QColor AlternateColor = QColor(36, 39, 46);
            QColor ScrollTrackColor = QColor(28, 30, 35);
            QColor ScrollThumbColor = QColor(100, 110, 130);
            std::function<void(int, const QString&)> OnSelectionChanged;

        private:
            bool isScrollBarVisible() const;
            bool isPointOnScrollTrack(const Vector &point) const;
            bool isPointOnScrollThumb(const Vector &point) const;
            void setScrollFromThumbTop(pe_float_t thumbTopFromWindowBottom);
    };
}

#endif // LISTVIEW_H
