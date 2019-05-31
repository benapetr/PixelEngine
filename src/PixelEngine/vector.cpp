//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019
#include "vector.h"
#include "definitions.h"
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

double Vector::AngleTo(const Vector &v)
{
    double delta_x = v.X - this->X;
    double delta_y = v.Y - this->Y;
    return std::atan2(delta_y, delta_x) / PE_PI_RAD_CNV;
}
