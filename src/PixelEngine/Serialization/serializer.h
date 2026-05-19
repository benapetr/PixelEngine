//! \file serializer.h
//! \brief Header file for the Serializer interface.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "../definitions.h"
#include "../vector.h"
#include <QString>
#include <QVariant>

namespace PE
{
    //! \class Serializer
    //! \brief Format-agnostic writer for serializable authored state.
    class Serializer
    {
        public:
            virtual ~Serializer() {}
            virtual void WriteValue(const QString &name, const QVariant &value) = 0;
            void WriteString(const QString &name, const QString &value);
            void WriteBool(const QString &name, bool value);
            void WriteInteger(const QString &name, int value);
            void WriteFloat(const QString &name, pe_float_t value);
            void WriteVector(const QString &name, const Vector &value);
    };
}

#endif // SERIALIZER_H
