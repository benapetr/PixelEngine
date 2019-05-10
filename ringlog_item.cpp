//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "ringlog_item.h"

using namespace PE;

RingLog_Item::RingLog_Item(const QString &Text)
{
    this->ts = QDateTime::currentDateTime();
    this->text = Text;
}

QString RingLog_Item::GetText()
{
    return this->text;
}

QDateTime RingLog_Item::GetTS()
{
    return this->ts;
}
