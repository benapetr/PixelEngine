// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef RESOURCES_H
#define RESOURCES_H

#include <QPixmap>
#include <QHash>

namespace PE
{
    class Resources
    {
        public:
            static const QPixmap &GetPixmap(const QString &name);
            static void ClearCache();
            static qint64 GetSize();
        private:
            static QHash<QString, QPixmap> pixmaps;
            static qint64 resourcesSize;
    };
}

#endif // RESOURCES_H
