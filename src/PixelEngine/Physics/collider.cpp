//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include <cmath>
#include "collider.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"

using namespace PE;

#ifdef PE_DEBUG
bool Collider::Debug = false;
#endif
#ifdef PE_METRICS
int Collider::Count = 0;
#endif

Collider::Collider(Object *parent) : Object(parent)
{
#ifdef PE_METRICS
    Count++;
#endif
}

Collider::~Collider()
{
#ifdef PE_METRICS
    Count--;
#endif
}

PE_ObjectType Collider::GetType()
{
    return PE_ObjectType_Collider;
}

bool Collider::IsOriented() const
{
    return std::abs(this->GetWorldRotation()) > static_cast<pe_float_t>(0.001);
}

QString Collider::GetClassName() const
{
    return "PE::Collider";
}

void Collider::Serialize(Serializer *serializer) const
{
    Object::Serialize(serializer);
    serializer->WriteInteger("layer", static_cast<int>(this->Layer));
    serializer->WriteBool("blocksNavigation", this->BlocksNavigation);
}

void Collider::Deserialize(Deserializer *deserializer)
{
    Object::Deserialize(deserializer);
    this->Layer = static_cast<unsigned int>(deserializer->ReadInteger("layer", static_cast<int>(this->Layer)));
    this->BlocksNavigation = deserializer->ReadBool("blocksNavigation", this->BlocksNavigation);
}
