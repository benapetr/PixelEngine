//! \file containerentry.h
//! \brief Header file for generic asset container entries.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef CONTAINERENTRY_H
#define CONTAINERENTRY_H

#include <QByteArray>
#include <QString>
#include <QVariantMap>

namespace PE
{
    //! \enum PE_ContainerEntryType
    //! \brief Describes what kind of payload is stored in a container entry.
    enum PE_ContainerEntryType
    {
        PE_ContainerEntryType_Unknown,
        PE_ContainerEntryType_SerializedObject,
        PE_ContainerEntryType_BinaryBlob,
        PE_ContainerEntryType_AssetReference,
        PE_ContainerEntryType_Container
    };

    //! \class ContainerEntry
    //! \brief Generic entry stored inside an asset container.
    class ContainerEntry
    {
        public:
            ContainerEntry();

            static QString EntryTypeToString(PE_ContainerEntryType type);
            static PE_ContainerEntryType EntryTypeFromString(const QString &type);
            QVariantMap ToVariantMap() const;
            static ContainerEntry FromVariantMap(const QVariantMap &map);

            QString Id;
            QString Name;
            QString Description;
            QString ClassName;
            QString Encoding;
            QString ReferenceId;
            QVariantMap Metadata;
            QByteArray Data;
            PE_ContainerEntryType Type = PE_ContainerEntryType_Unknown;
    };
}

#endif // CONTAINERENTRY_H
