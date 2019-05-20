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
    Vector movement = this->Velocity;
    movement.Y += -1 * this->GravityForce;
    return movement;
}

void Rigidbody::ResetForceAfterImpact()
{
    this->GravityForce = 0;
    this->Velocity = Vector::Zero;
}
