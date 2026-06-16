//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef SPRITE_H
#define SPRITE_H

#include "../object.h"
#include <QPixmap>

namespace PE
{
    class Sprite : public Object
    {
        public:
            Sprite();
            Sprite(const Vector &position, int width, int height, const QString &resourceName = QString(), Object *parent = nullptr);
            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            void Render(Renderer *r, Camera *c) override;
            void SetPixmap(const QPixmap &pixmap);
            void SetResourceName(const QString &resourceName);
            void SetImageResource(const QString &entryId, const QString &packagePath = QString());

            QString ResourceName;
            QString ImagePackagePath;
            QString ImageEntryId;
            QPixmap Pixmap;
            int Width = 0;
            int Height = 0;
    };
}

#endif // SPRITE_H
