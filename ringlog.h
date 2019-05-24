// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef RINGLOG_H
#define RINGLOG_H

#include <QList>
#include <QObject>
#include <QString>
#include "ringlog_item.h"

namespace PE
{
    class RingLog : public QObject
    {
            Q_OBJECT
        public:
            RingLog();
            void WriteText(const QString &text);
            QList<RingLog_Item> GetItems();

        private:
            int size = 2000;
            QList<RingLog_Item> items;
    };
}

#endif // RINGLOG_H
