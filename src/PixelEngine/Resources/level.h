//! \file level.h
//! \brief Header file for serializable level definitions.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef LEVEL_H
#define LEVEL_H

#include "../Serialization/serializable.h"
#include "../vector.h"
#include "../definitions.h"
#include <QColor>
#include <QList>
#include <QString>
#include <QVariantMap>

namespace PE
{
    class World;

    class LevelPlacement
    {
        public:
            QVariantMap ToVariantMap() const;
            static LevelPlacement FromVariantMap(const QVariantMap &map);

            QString Id;
            QString PackagePath;
            QString EntryId;
            Vector Position;
            pe_float_t Rotation = 0;
            pe_float_t Scale = 1;
            int ZIndex = 1;
    };

    //! \class Level
    //! \brief Serializable world/level definition stored inside .pe asset containers.
    class Level : public Serializable
    {
        public:
            Level();
            Level(const QString &id, const QString &name, pe_float_t width, pe_float_t height);

            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            World *CreateWorld() const;

            QString Id;
            QString Name;
            QString Description;
            pe_float_t Width = 1280;
            pe_float_t Height = 720;
            pe_float_t Gravity = 1;
            QColor BackgroundColor = QColor(22, 26, 34);
            QColor BorderColor = QColor(82, 166, 255);
            bool HasBorder = true;
            QList<LevelPlacement> Placements;
    };
}

#endif // LEVEL_H
