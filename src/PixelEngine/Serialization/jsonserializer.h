//! \file jsonserializer.h
//! \brief Header file for JSON serialization.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "serializer.h"
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

namespace PE
{
    //! \class JsonSerializer
    //! \brief Serializer backend that writes values to a JSON object.
    class JsonSerializer : public Serializer
    {
        public:
            void WriteValue(const QString &name, const QVariant &value) override;
            QJsonObject ToJsonObject() const;
            QJsonDocument ToJsonDocument() const;
            QByteArray ToJson(QJsonDocument::JsonFormat format = QJsonDocument::Indented) const;

        private:
            QVariantMap values;
    };
}

#endif // JSONSERIALIZER_H
