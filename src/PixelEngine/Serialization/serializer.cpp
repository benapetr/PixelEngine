//! \file serializer.cpp
//! \brief Implementation of the Serializer interface helpers.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "serializer.h"
#include <QVariantList>

using namespace PE;

void Serializer::WriteString(const QString &name, const QString &value)
{
    this->WriteValue(name, value);
}

void Serializer::WriteBool(const QString &name, bool value)
{
    this->WriteValue(name, value);
}

void Serializer::WriteInteger(const QString &name, int value)
{
    this->WriteValue(name, value);
}

void Serializer::WriteFloat(const QString &name, pe_float_t value)
{
    this->WriteValue(name, static_cast<double>(value));
}

void Serializer::WriteVector(const QString &name, const Vector &value)
{
    QVariantList vector;
    vector.append(static_cast<double>(value.X));
    vector.append(static_cast<double>(value.Y));
    this->WriteValue(name, vector);
}
