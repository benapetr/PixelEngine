// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
}

void Rigidbody::ResetForceAfterImpact()
{
    this->Velocity = Vector::Zero;
}
