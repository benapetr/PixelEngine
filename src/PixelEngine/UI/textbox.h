//! \file textbox.h
//! \brief Header file for UI text box.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "uielement.h"

namespace PE
{
    class TextBox : public UIElement
    {
        public:
            TextBox();
            TextBox(const Vector &position, pe_float_t width, pe_float_t height, Object *parent = nullptr);
            QString GetClassName() const override;
            void Render(Renderer *r, Camera *c) override;
            void MousePress(const Vector &point) override;
            void KeyPress(int key, const QString &text = QString()) override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;

            QString Text;
            QString Placeholder;
            int FontSize = 13;
            int MaxLength = 256;
            bool Editable = true;
    };
}

#endif // TEXTBOX_H
