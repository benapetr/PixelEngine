//! \file prefab.h
//! \brief Header file for reusable object prefabs.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef PREFAB_H
#define PREFAB_H

#include "../Serialization/serializable.h"
#include <QVariantMap>

namespace PE
{
    class Object;

    //! \class Prefab
    //! \brief Serializable reusable object definition.
    class Prefab : public Serializable
    {
        public:
            Prefab();
            Prefab(const QString &id, const QString &name, const Object *root = nullptr);

            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            void SetRootObject(const Object *root);
            Object *CreateObject() const;

            QString Id;
            QString Name;
            QString Description;
            QString RootClassName;
            QVariantMap RootObject;
    };
}

#endif // PREFAB_H
