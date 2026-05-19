//! \file assetcontainer.h
//! \brief Header file for generic asset containers.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef ASSETCONTAINER_H
#define ASSETCONTAINER_H

#include "../Serialization/serializable.h"
#include "containerentry.h"
#include <QList>

namespace PE
{
    class Object;

    //! \class AssetContainer
    //! \brief Named serializable collection of generic assets.
    class AssetContainer : public Serializable
    {
        public:
            AssetContainer();
            AssetContainer(const QString &id, const QString &name, const QString &description = QString());

            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;

            void AddEntry(const ContainerEntry &entry);
            ContainerEntry *GetEntry(const QString &id);
            const ContainerEntry *GetEntry(const QString &id) const;
            bool HasEntry(const QString &id) const;
            void RemoveEntry(const QString &id);

            void AddBinary(const QString &id, const QString &name, const QByteArray &data, const QString &encoding, const QString &description = QString());
            void AddReference(const QString &id, const QString &name, const QString &referenceId, const QString &description = QString());
            void AddSerializable(const QString &id, const QString &name, const Serializable *object, const QString &description = QString());
            Serializable *CreateSerializable(const QString &id) const;
            Object *CreateObject(const QString &id) const;

            bool SaveToFile(const QString &path) const;
            static AssetContainer *LoadFromFile(const QString &path);

            QString Id;
            QString Name;
            QString Description;
            QList<ContainerEntry> Entries;
    };
}

#endif // ASSETCONTAINER_H
