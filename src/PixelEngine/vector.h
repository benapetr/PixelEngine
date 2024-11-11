//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef VECTOR_H
#define VECTOR_H

#include "definitions.h"

namespace PE
{
    class Vector
    {
        public:
            static const Vector Zero;
            static const Vector Top;
            static const Vector Left;
            static const Vector Right;
            static const Vector Down;

            Vector();
            Vector(pe_float_t x, pe_float_t y);
            virtual ~Vector();
            int X2int() { return static_cast<int>(this->X); }
            int Y2int() { return static_cast<int>(this->Y); }
            pe_float_t Magnitude();
            pe_float_t DistanceTo(const Vector &v);
            pe_float_t AngleTo(const Vector &v);
            pe_float_t Dot();
            pe_float_t Dot(const Vector &v);
            pe_float_t X=0;
            pe_float_t Y=0;

            // operators
            Vector operator+(const Vector &v)
            {
                return {this->X + v.X, this->Y + v.Y};
            }

            void operator+=(const Vector &v)
            {
                this->X += v.X;
                this->Y += v.Y;
            }

            void operator-=(const Vector &v)
            {
                this->X -= v.X;
                this->Y -= v.Y;
            }

            void operator+=(const pe_float_t scalar)
            {
                this->X += scalar;
                this->Y += scalar;
            }

            void operator-=(const pe_float_t scalar)
            {
                this->X -= scalar;
                this->Y -= scalar;
            }

            Vector operator-(const Vector &v)
            {
                return {this->X - v.X, this->Y - v.Y};
            }

            Vector operator*(const pe_float_t scalar)
            {
                return {this->X * scalar, this->Y * scalar};
            }

            Vector operator/(const pe_float_t scalar)
            {
                return {this->X / scalar, this->Y / scalar};
            }

            Vector operator+(const pe_float_t scalar)
            {
                return {this->X + scalar, this->Y + scalar};
            }

            Vector operator-(const pe_float_t scalar)
            {
                return {this->X - scalar, this->Y - scalar};
            }

            bool operator!=(const Vector &v)
            {
                return this->X != v.X || this->Y != v.Y;
            }

            bool operator==(const Vector &v)
            {
                return this->X == v.X && this->Y == v.Y;
            }
    };
}

#endif // VECTOR_H
