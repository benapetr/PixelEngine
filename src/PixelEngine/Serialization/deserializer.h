//! \file deserializer.h
//! \brief Header file for the Deserializer interface.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include "../definitions.h"
#include "../vector.h"
#include <QString>
#include <QVariant>

namespace PE
{
    //! \class Deserializer
    //! \brief Format-agnostic reader for serializable authored state.
    class Deserializer
    {
        public:
            virtual ~Deserializer() {}
            virtual bool HasValue(const QString &name) const = 0;
            virtual QVariant ReadValue(const QString &name, const QVariant &defaultValue = QVariant()) const = 0;
            QString ReadString(const QString &name, const QString &defaultValue = QString()) const;
            bool ReadBool(const QString &name, bool defaultValue = false) const;
            int ReadInteger(const QString &name, int defaultValue = 0) const;
            pe_float_t ReadFloat(const QString &name, pe_float_t defaultValue = 0) const;
            Vector ReadVector(const QString &name, const Vector &defaultValue = Vector()) const;
    };
}

#endif // DESERIALIZER_H
