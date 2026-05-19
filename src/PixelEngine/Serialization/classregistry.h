//! \file classregistry.h
//! \brief Header file for serializable class registration.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef CLASSREGISTRY_H
#define CLASSREGISTRY_H

#include "serializable.h"
#include <QHash>
#include <QString>
#include <functional>

namespace PE
{
    //! \class ClassRegistry
    //! \brief Factory registry for serializable classes.
    class ClassRegistry
    {
        public:
            typedef std::function<Serializable*()> CreateFunction;

            static ClassRegistry *GetRegistry();
            void RegisterClass(const QString &className, CreateFunction createFunction);
            bool HasClass(const QString &className) const;
            Serializable *Create(const QString &className) const;

        private:
            QHash<QString, CreateFunction> createFunctions;
    };
}

#endif // CLASSREGISTRY_H
