// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "vector.h"
#include <cmath>

using namespace PE;

const Vector Vector::Zero = Vector(0, 0);
const Vector Vector::Top = Vector(0, 1);
const Vector Vector::Down = Vector(0, -1);
const Vector Vector::Left = Vector(-1, 0);
const Vector Vector::Right = Vector(1, 0);

Vector::Vector()
{

}

Vector::Vector(double x, double y)
{
    this->X = x;
    this->Y = y;
}

Vector::~Vector()
{

}

double Vector::Magnitude()
{
    return std::sqrt((this->X * this->X) + (this->Y * this->Y));
}

double Vector::DistanceTo(const Vector &v)
{
    return std::sqrt(std::pow(this->X - v.X, 2) + std::pow(this->Y - v.Y, 2));
}
