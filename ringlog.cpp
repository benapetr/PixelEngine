// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "ringlog.h"

using namespace PE;

RingLog::RingLog()
{

}

void RingLog::WriteText(const QString& text)
{
    while (this->items.size() > this->size)
        this->items.removeFirst();
    this->items.append(RingLog_Item(text));
}

QList<RingLog_Item> RingLog::GetItems()
{
    return this->items;
}

