// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
