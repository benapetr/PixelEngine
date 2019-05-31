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
    class RingLog_Item
    {
        public:
            RingLog_Item(const QString &Text);
            QString GetText();
            QDateTime GetTS();
        private:
            QString text;
            QDateTime ts;
    };
}

#endif // RINGLOG_ITEM_H
