//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019
//! \file ringlog.h
//! \brief Header file for the RingLog class, managing a circular log of messages.

#ifndef RINGLOG_H
#define RINGLOG_H

#include <QList>
#include <QObject>
#include <QString>
#include "ringlog_item.h"

namespace PE
{
    //! \class RingLog
    //! \brief Manages a circular log of messages for the Pixel Engine.
    class RingLog : public QObject
    {
            Q_OBJECT
        public:
            //! \brief Constructor for the RingLog class.
            RingLog();

            //! \brief Write a text message to the log.
            //! \param text The text message to write.
            void WriteText(const QString &text);

            //! \brief Retrieve all log items.
            //! \return A QList of RingLog_Item objects.
            QList<RingLog_Item> GetItems();

        private:
            int size = 2000; //!< The maximum size of the log.
            QList<RingLog_Item> items; //!< The list of log items.
    };
}

#endif // RINGLOG_H
