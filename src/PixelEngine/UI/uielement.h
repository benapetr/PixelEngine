//! \file uielement.h
//! \brief Header file for base UI element class.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "../object.h"
#include <QColor>
#include <QString>

namespace PE
{
    class UIElement : public Object
    {
        public:
            UIElement();
            UIElement(const Vector &position, pe_float_t width, pe_float_t height, Object *parent = nullptr);
            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            virtual bool ContainsPoint(const Vector &point) const;
            virtual void MousePress(const Vector &point);
            virtual void MouseRelease(const Vector &point);
            virtual void MouseMove(const Vector &point);
            virtual void KeyPress(int key, const QString &text = QString());

            pe_float_t Width = 0;
            pe_float_t Height = 0;
            bool Enabled = true;
            bool Visible = true;
            bool Focused = false;
            QColor BackgroundColor = QColor(43, 45, 51);
            QColor BorderColor = QColor(110, 116, 128);
            QColor TextColor = QColor(240, 242, 247);
    };
}

#endif // UIELEMENT_H
