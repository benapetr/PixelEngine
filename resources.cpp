// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "resources.h"

using namespace PE;

QHash<QString, QPixmap> Resources::pixmaps;
qint64 Resources::resourcesSize = 0;

const QPixmap &Resources::GetPixmap(const QString &name)
{
    if (!Resources::pixmaps.contains(name))
    {
        QPixmap p(name);
        Resources::pixmaps.insert(name, p);
        Resources::resourcesSize += p.size().width() * p.size().height() * 4;
    }

    return Resources::pixmaps[name];
}

void Resources::ClearCache()
{
    Resources::resourcesSize = 0;
    Resources::pixmaps.clear();
}

qint64 Resources::GetSize()
{
    return Resources::resourcesSize;
}
