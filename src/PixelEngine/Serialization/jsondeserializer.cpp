//! \file jsondeserializer.cpp
//! \brief Implementation of JSON deserialization.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "jsondeserializer.h"
#include <QJsonParseError>

using namespace PE;

JsonDeserializer::JsonDeserializer()
{

}

JsonDeserializer::JsonDeserializer(const QJsonObject &object)
{
    this->values = object.toVariantMap();
}

JsonDeserializer::JsonDeserializer(const QJsonDocument &document)
{
    this->valid = document.isObject();
    if (this->valid)
        this->values = document.object().toVariantMap();
}

JsonDeserializer::JsonDeserializer(const QByteArray &json)
{
    QJsonParseError error;
    QJsonDocument document = QJsonDocument::fromJson(json, &error);
    this->valid = (error.error == QJsonParseError::NoError && document.isObject());
    if (this->valid)
        this->values = document.object().toVariantMap();
}

bool JsonDeserializer::HasValue(const QString &name) const
{
    return this->values.contains(name);
}

QVariant JsonDeserializer::ReadValue(const QString &name, const QVariant &defaultValue) const
{
    if (!this->values.contains(name))
        return defaultValue;
    return this->values.value(name);
}

bool JsonDeserializer::IsValid() const
{
    return this->valid;
}
