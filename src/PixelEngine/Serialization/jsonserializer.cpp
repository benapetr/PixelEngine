//! \file jsonserializer.cpp
//! \brief Implementation of JSON serialization.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "jsonserializer.h"
#include <QJsonValue>

using namespace PE;

void JsonSerializer::WriteValue(const QString &name, const QVariant &value)
{
    this->values.insert(name, value);
}

QJsonObject JsonSerializer::ToJsonObject() const
{
    return QJsonObject::fromVariantMap(this->values);
}

QJsonDocument JsonSerializer::ToJsonDocument() const
{
    return QJsonDocument(this->ToJsonObject());
}

QByteArray JsonSerializer::ToJson(QJsonDocument::JsonFormat format) const
{
    return this->ToJsonDocument().toJson(format);
}
