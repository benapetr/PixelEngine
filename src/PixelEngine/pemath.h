//! \file pemath.h
//! \brief Header file for the PEMath class, providing mathematical utilities for the engine.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef PEMATH_H
#define PEMATH_H

#include <QtGlobal>

namespace PE
{
    //! \class PEMath
    //! \brief Provides mathematical utility functions for the Pixel Engine.
    class PEMath
    {
        public:
            //! \brief Generate a random integer within a specified range.
            //! \param min The minimum value of the range.
            //! \param max The maximum value of the range.
            //! \return A random integer between min and max (inclusive).
            static qint32 GetRandom(qint32 min, qint32 max);
    };
}

#endif // PEMATH_H
