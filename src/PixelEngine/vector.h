//! \file vector.h
//! \brief Header file for the Vector class, representing 2D vectors in the Pixel Engine.

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
    //! \class Vector
    //! \brief Represents a 2D vector with various mathematical operations.
    class Vector
    {
        public:
            //! \brief A constant representing the zero vector (0, 0).
            static const Vector Zero;

            //! \brief A constant representing the top direction vector (0, -1).
            static const Vector Top;

            //! \brief A constant representing the left direction vector (-1, 0).
            static const Vector Left;

            //! \brief A constant representing the right direction vector (1, 0).
            static const Vector Right;

            //! \brief A constant representing the down direction vector (0, 1).
            static const Vector Down;

            //! \brief Default constructor for the Vector class.
            Vector();

            //! \brief Constructor for the Vector class with specified coordinates.
            //! \param x The x-coordinate of the vector.
            //! \param y The y-coordinate of the vector.
            Vector(pe_float_t x, pe_float_t y);

            //! \brief Destructor for the Vector class.
            virtual ~Vector();

            //! \brief Convert the x-coordinate to an integer.
            //! \return The x-coordinate as an integer.
            int X2int() { return static_cast<int>(this->X); }

            //! \brief Convert the y-coordinate to an integer.
            //! \return The y-coordinate as an integer.
            int Y2int() { return static_cast<int>(this->Y); }

            //! \brief Calculate the magnitude (length) of the vector.
            //! \return The magnitude of the vector.
            pe_float_t Magnitude();

            //! \brief Calculate the distance to another vector.
            //! \param v The other vector.
            //! \return The distance to the other vector.
            pe_float_t DistanceTo(const Vector &v);

            //! \brief Calculate the angle to another vector.
            //! \param v The other vector.
            //! \return The angle to the other vector in radians.
            pe_float_t AngleTo(const Vector &v);

            //! \brief Calculate the dot product of the vector with itself.
            //! \return The dot product.
            pe_float_t Dot();

            //! \brief Calculate the dot product with another vector.
            //! \param v The other vector.
            //! \return The dot product.
            pe_float_t Dot(const Vector &v);

            pe_float_t X = 0; //!< The x-coordinate of the vector.
            pe_float_t Y = 0; //!< The y-coordinate of the vector.

            // operators
            Vector operator+(const Vector &v) { return {this->X + v.X, this->Y + v.Y}; }
            void operator+=(const Vector &v) { this->X += v.X; this->Y += v.Y; }
            void operator-=(const Vector &v) { this->X -= v.X; this->Y -= v.Y; }
            void operator+=(const pe_float_t scalar) { this->X += scalar; this->Y += scalar; }
            void operator-=(const pe_float_t scalar) { this->X -= scalar; this->Y -= scalar; }
            Vector operator-(const Vector &v) { return {this->X - v.X, this->Y - v.Y}; }
            Vector operator*(const pe_float_t scalar) { return {this->X * scalar, this->Y * scalar}; }
            Vector operator/(const pe_float_t scalar) { return {this->X / scalar, this->Y / scalar}; }
            Vector operator+(const pe_float_t scalar) { return {this->X + scalar, this->Y + scalar}; }
            Vector operator-(const pe_float_t scalar) { return {this->X - scalar, this->Y - scalar}; }
            bool operator!=(const Vector &v) { return this->X != v.X || this->Y != v.Y; }
            bool operator==(const Vector &v) { return this->X == v.X && this->Y == v.Y; }
    };
}

#endif // VECTOR_H
