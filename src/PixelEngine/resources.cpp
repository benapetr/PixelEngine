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
#include "Resources/assetcontainer.h"
#include "Resources/containerentry.h"
#include "resources.h"

using namespace PE;

QHash<QString, QPixmap> Resources::pixmaps;
QHash<QString, AssetContainer*> Resources::assetContainers;
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

const QPixmap &Resources::GetPixmap(const QString &packagePath, const QString &entryId)
{
    QString cacheKey = "pe:" + packagePath + ":" + entryId;
    if (!Resources::pixmaps.contains(cacheKey))
    {
        QPixmap pixmap;
        const ContainerEntry *entry = nullptr;

        if (!packagePath.isEmpty() && Resources::assetContainers.contains(packagePath))
        {
            entry = Resources::assetContainers[packagePath]->GetEntry(entryId);
        } else {
            for (AssetContainer *container : Resources::assetContainers)
            {
                if (!container)
                    continue;
                entry = container->GetEntry(entryId);
                if (entry)
                    break;
            }
        }

        if (entry && entry->Type == PE_ContainerEntryType_BinaryBlob)
            pixmap.loadFromData(entry->Data);

        Resources::pixmaps.insert(cacheKey, pixmap);
        Resources::resourcesSize += pixmap.size().width() * pixmap.size().height() * 4;
    }

    return Resources::pixmaps[cacheKey];
}

void Resources::MountAssetContainer(const QString &packagePath, AssetContainer *container)
{
    if (packagePath.isEmpty() || !container)
        return;

    Resources::assetContainers.insert(packagePath, container);
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
