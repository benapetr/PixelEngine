// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "camera.h"

using namespace PE;

Camera::Camera()
{

}

Vector Camera::GetNegated()
{
    return this->Position * -1;
}

Vector Camera::ProjectedPosition(Vector position)
{
    return position + (this->Position * -1);
}
