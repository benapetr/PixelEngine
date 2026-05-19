//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "rigidbody.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"

using namespace PE;

Rigidbody::Rigidbody()
{

}

QString Rigidbody::GetClassName() const
{
    return "PE::Rigidbody";
}

void Rigidbody::Serialize(Serializer *serializer) const
{
    serializer->WriteString("class", this->GetClassName());
    serializer->WriteVector("velocity", this->Velocity);
    serializer->WriteFloat("bounciness", this->Bounciness);
    serializer->WriteFloat("weight", this->Weight);
}

void Rigidbody::Deserialize(Deserializer *deserializer)
{
    this->Velocity = deserializer->ReadVector("velocity", this->Velocity);
    this->Bounciness = deserializer->ReadFloat("bounciness", this->Bounciness);
    this->Weight = deserializer->ReadFloat("weight", this->Weight);
    this->GroundCollider = nullptr;
}

bool Rigidbody::IsGrounded()
{
    return this->GroundCollider != nullptr;
}

Vector Rigidbody::GetMovement()
{
    return this->Velocity;
}

void Rigidbody::AddForce(const Vector &force)
{
    this->Velocity += force;
    // Drop info about ground collider
    this->GroundCollider = nullptr;
}

void Rigidbody::ResetForceAfterImpact()
{
    this->Velocity = Vector::Zero;
}
