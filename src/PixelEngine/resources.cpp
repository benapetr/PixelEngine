//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019 - 2024

#include <QFile>
#include <QDebug>
#include <QTextStream>
#include "engine.h"
#include "ringlog.h"
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

const QString Resources::GetText(const QString &name)
{
    QFile file(name);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        Engine::GetEngine()->RL->WriteText("WARNING: Could not open resource text file: " + name);
        return QString();
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    return fileContent;
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
