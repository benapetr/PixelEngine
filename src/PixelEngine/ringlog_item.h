//! \file ringlog_item.h
//! \brief Header file for the RingLog_Item class, representing a single log entry in the ring log.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef RINGLOG_ITEM_H
#define RINGLOG_ITEM_H

#include <QDateTime>
#include <QString>

namespace PE
{
    //! \class RingLog_Item
    //! \brief Represents a single log entry in the ring log.
    class RingLog_Item
    {
        public:
            //! \brief Constructor for the RingLog_Item class.
            //! \param Text The text of the log entry.
            RingLog_Item(const QString &Text);

            //! \brief Get the text of the log entry.
            //! \return A QString containing the log text.
            QString GetText();

            //! \brief Get the timestamp of the log entry.
            //! \return A QDateTime object representing the timestamp.
            QDateTime GetTS();

        private:
            QString text; //!< The text of the log entry.
            QDateTime ts; //!< The timestamp of the log entry.
    };
}

#endif // RINGLOG_ITEM_H
