// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
