//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019 - 2024

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
            static const QString GetText(const QString &name);
            static void ClearCache();
            static qint64 GetSize();
        private:
            static QHash<QString, QPixmap> pixmaps;
            static qint64 resourcesSize;
    };
}

#endif // RESOURCES_H
