// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
            int X2int() { return static_cast<int>(this->X); }
            int Y2int() { return static_cast<int>(this->Y); }
            double Magnitude();
            double X=0;
            double Y=0;

            // operators
            Vector operator+(const Vector &v)
            {
                return {this->X + v.X, this->Y + v.Y};
            }

            Vector operator-(const Vector &v)
            {
                return {this->X - v.X, this->Y - v.Y};
            }

            Vector operator*(const double scalar)
            {
                return {this->X * scalar, this->Y * scalar};
            }
            bool operator!=(const Vector &v)
            {
                return this->X != v.X && this->Y != v.Y;
            }
            bool operator==(const Vector &v)
            {
                return this->X == v.X && this->Y == v.Y;
            }
    };
}

#endif // VECTOR_H
