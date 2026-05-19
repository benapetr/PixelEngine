//! \file serializable.h
//! \brief Header file for the Serializable interface.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QString>

namespace PE
{
    class Serializer;
    class Deserializer;

    //! \class Serializable
    //! \brief Interface for objects that can persist their authored state.
    class Serializable
    {
        public:
            virtual ~Serializable() {}
            virtual QString GetClassName() const = 0;
            virtual void Serialize(Serializer *serializer) const = 0;
            virtual void Deserialize(Deserializer *deserializer) = 0;
    };
}

#endif // SERIALIZABLE_H
