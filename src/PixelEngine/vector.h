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
            Vector(double x, double y);
            virtual ~Vector();
            int X2int() { return static_cast<int>(this->X); }
            int Y2int() { return static_cast<int>(this->Y); }
            double Magnitude();
            double DistanceTo(const Vector &v);
            double AngleTo(const Vector &v);
            double Dot();
            double Dot(const Vector &v);
            double X=0;
            double Y=0;

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

            void operator+=(const double scalar)
            {
                this->X += scalar;
                this->Y += scalar;
            }

            void operator-=(const double scalar)
            {
                this->X -= scalar;
                this->Y -= scalar;
            }

            Vector operator-(const Vector &v)
            {
                return {this->X - v.X, this->Y - v.Y};
            }

            Vector operator*(const double scalar)
            {
                return {this->X * scalar, this->Y * scalar};
            }

            Vector operator/(const double scalar)
            {
                return {this->X / scalar, this->Y / scalar};
            }

            Vector operator+(const double scalar)
            {
                return {this->X + scalar, this->Y + scalar};
            }

            Vector operator-(const double scalar)
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
