//! \file deserializer.cpp
//! \brief Implementation of the Deserializer interface helpers.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "deserializer.h"
#include <QVariantList>

using namespace PE;

QString Deserializer::ReadString(const QString &name, const QString &defaultValue) const
{
    return this->ReadValue(name, defaultValue).toString();
}

bool Deserializer::ReadBool(const QString &name, bool defaultValue) const
{
    return this->ReadValue(name, defaultValue).toBool();
}

int Deserializer::ReadInteger(const QString &name, int defaultValue) const
{
    return this->ReadValue(name, defaultValue).toInt();
}

pe_float_t Deserializer::ReadFloat(const QString &name, pe_float_t defaultValue) const
{
    return static_cast<pe_float_t>(this->ReadValue(name, static_cast<double>(defaultValue)).toDouble());
}

Vector Deserializer::ReadVector(const QString &name, const Vector &defaultValue) const
{
    QVariant value = this->ReadValue(name);
    QVariantList vector = value.toList();
    if (vector.size() != 2)
        return defaultValue;
    return Vector(static_cast<pe_float_t>(vector[0].toDouble()), static_cast<pe_float_t>(vector[1].toDouble()));
}
