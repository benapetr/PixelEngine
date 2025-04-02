//! \file generic.h
//! \brief Header file for the Generic namespace, providing utility functions for common operations.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef GENERIC_H
#define GENERIC_H

#include <QString>

namespace PE
{
    namespace Generic
    {
        //! \brief Convert a boolean value to a string representation ("True" or "False").
        //! \param value The boolean value to convert.
        //! \return A QString containing "True" or "False".
        QString Bool2String(bool value);

        //! \brief Convert a boolean value to a lowercase string representation ("true" or "false").
        //! \param value The boolean value to convert.
        //! \return A QString containing "true" or "false".
        QString Bool2LString(bool value);
    }
}

#endif // GENERIC_H
