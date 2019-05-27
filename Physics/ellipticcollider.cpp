// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "ellipticcollider.h"

using namespace PE;

EllipticCollider::EllipticCollider(double x, double y, double rx, double ry)
{
    this->RadiusX = rx;
    this->Position = Vector(x, y);
    this->RadiusY = ry;
}
