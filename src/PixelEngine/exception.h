//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

//! \file exception.h
//! \brief Header file for the Exception class, representing errors in the Pixel Engine.

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace PE
{
    //! \class Exception
    //! \brief Represents an exception in the Pixel Engine.
    class Exception
    {
        public:
            //! \brief Constructor for the Exception class.
            //! \param what A description of what caused the exception.
            //! \param where The location where the exception occurred.
            Exception(const QString &what, const QString &where);

            //! \brief Destructor for the Exception class.
            virtual ~Exception();

            QString What; //!< A description of what caused the exception.
            QString Where; //!< The location where the exception occurred.
    };
}

#endif // EXCEPTION_H
