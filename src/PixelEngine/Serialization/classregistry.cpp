//! \file classregistry.cpp
//! \brief Implementation of serializable class registration.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "classregistry.h"

using namespace PE;

ClassRegistry *ClassRegistry::GetRegistry()
{
    static ClassRegistry registry;
    return &registry;
}

void ClassRegistry::RegisterClass(const QString &className, CreateFunction createFunction)
{
    this->createFunctions.insert(className, createFunction);
}

bool ClassRegistry::HasClass(const QString &className) const
{
    return this->createFunctions.contains(className);
}

Serializable *ClassRegistry::Create(const QString &className) const
{
    if (!this->createFunctions.contains(className))
        return nullptr;
    return this->createFunctions.value(className)();
}
