// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "collider.h"

using namespace PE;

Collider::Collider()
{

}

PE_ObjectType Collider::GetType()
{
    return PE_ObjectType_Collider;
}
