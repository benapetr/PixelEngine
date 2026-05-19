//! \file jsondeserializer.h
//! \brief Header file for JSON deserialization.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef JSONDESERIALIZER_H
#define JSONDESERIALIZER_H

#include "deserializer.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

namespace PE
{
    //! \class JsonDeserializer
    //! \brief Deserializer backend that reads values from a JSON object.
    class JsonDeserializer : public Deserializer
    {
        public:
            JsonDeserializer();
            explicit JsonDeserializer(const QJsonObject &object);
            explicit JsonDeserializer(const QJsonDocument &document);
            explicit JsonDeserializer(const QByteArray &json);
            bool HasValue(const QString &name) const override;
            QVariant ReadValue(const QString &name, const QVariant &defaultValue = QVariant()) const override;
            bool IsValid() const;

        private:
            QVariantMap values;
            bool valid = true;
    };
}

#endif // JSONDESERIALIZER_H
