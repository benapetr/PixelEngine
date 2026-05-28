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
#include <functional>

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
            void BringToFront(UIElement *control);
            void SendToBack(UIElement *control);
            void MoveForward(UIElement *control);
            void MoveBackward(UIElement *control);
            UIElement *GetFocusedControl() const;
            bool IsCloseButtonVisible() const;
            bool IsPointOnCloseButton(const Vector &point) const;
            void MousePress(const Vector &point) override;
            void MouseRelease(const Vector &point) override;
            void MouseMove(const Vector &point) override;
            bool MouseWheel(const Vector &point, pe_float_t delta) override;
            void KeyPress(int key, const QString &text = QString()) override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;

            QString Title;
            int TitleBarHeight = 26;
            int CloseButtonSize = 18;
            int FontSize = 13;
            bool ShowTitleBar = true;
            bool ShowCloseButton = false;
            QColor TitleBarColor = QColor(35, 38, 46);
            QColor CloseButtonColor = QColor(150, 78, 78);
            QColor CloseButtonHoverColor = QColor(190, 82, 82);
            QColor CloseButtonTextColor = QColor(250, 240, 240);
            bool CloseButtonHovered = false;
            bool CloseButtonPressed = false;
            std::function<void()> OnClose;

        private:
            int IndexOfControl(UIElement *control) const;
            QList<Collectable_SmartPtr<UIElement>> controls;
            Collectable_SmartPtr<UIElement> focusedControl;
    };
}

#endif // UI_WINDOW_H
