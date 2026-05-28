//! \file combobox.h
//! \brief Header file for UI combo box.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef COMBOBOX_H
#define COMBOBOX_H

#include "uielement.h"
#include <QStringList>
#include <functional>

namespace PE
{
    class ComboBox : public UIElement
    {
        public:
            ComboBox();
            ComboBox(const Vector &position, pe_float_t width, pe_float_t height, Object *parent = nullptr);
            QString GetClassName() const override;
            void Render(Renderer *r, Camera *c) override;
            bool ContainsPoint(const Vector &point) const override;
            void MousePress(const Vector &point) override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            void AddItem(const QString &item);
            void ClearItems();
            QString GetSelectedText() const;

            QStringList Items;
            int SelectedIndex = -1;
            int FontSize = 13;
            bool Expanded = false;
            std::function<void(int, const QString&)> OnSelectionChanged;
    };
}

#endif // COMBOBOX_H
