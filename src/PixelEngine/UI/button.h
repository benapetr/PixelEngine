//! \file button.h
//! \brief Header file for UI button.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef BUTTON_H
#define BUTTON_H

#include "uielement.h"
#include <functional>

namespace PE
{
    class Button : public UIElement
    {
        public:
            Button();
            Button(const QString &text, const Vector &position, pe_float_t width, pe_float_t height, Object *parent = nullptr);
            QString GetClassName() const override;
            void Render(Renderer *r, Camera *c) override;
            void MousePress(const Vector &point) override;
            void MouseRelease(const Vector &point) override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;

            QString Text;
            int FontSize = 13;
            QColor HoverColor = QColor(55, 59, 68);
            QColor PressedColor = QColor(78, 94, 126);
            bool Pressed = false;
            std::function<void()> OnClick;
    };
}

#endif // BUTTON_H
