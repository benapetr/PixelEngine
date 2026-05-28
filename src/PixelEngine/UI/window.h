//! \file window.h
//! \brief Header file for UI window.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include "uielement.h"
#include "../GC/collectable_smartptr.h"
#include <QList>

namespace PE
{
    class Window : public UIElement
    {
        public:
            Window();
            Window(const QString &title, const Vector &position, pe_float_t width, pe_float_t height, Object *parent = nullptr);
            QString GetClassName() const override;
            void Render(Renderer *r, Camera *c) override;
            void AddControl(UIElement *control);
            void RemoveControl(UIElement *control);
            UIElement *GetFocusedControl() const;
            void MousePress(const Vector &point) override;
            void MouseRelease(const Vector &point) override;
            void MouseMove(const Vector &point) override;
            void KeyPress(int key, const QString &text = QString()) override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;

            QString Title;
            int TitleBarHeight = 26;
            int FontSize = 13;
            QColor TitleBarColor = QColor(35, 38, 46);

        private:
            QList<Collectable_SmartPtr<UIElement>> controls;
            Collectable_SmartPtr<UIElement> focusedControl;
    };
}

#endif // UI_WINDOW_H
