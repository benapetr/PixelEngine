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

using namespace PE;

Rigidbody::Rigidbody()
{

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
