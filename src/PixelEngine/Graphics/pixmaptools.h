//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef PIXMAPTOOLS_H
#define PIXMAPTOOLS_H

#include <QPixmap>
#include <QVector>

namespace PE
{
    class PixmapTools
    {
        public:
            //! This is slow - do cache results
            static QPixmap Rotate(const QPixmap &pm, qreal angle);
            // In case there is a grid / border dividing sprites you can use separator_width to ignore this grid, it must be around each sprite
            static QVector<QPixmap> LoadTransparentSpritesFromTexture(const QPixmap &texture, int sprite_width, int sprite_height, QColor transparent, int separator_width = 0);
            static QVector<QPixmap> LoadSpritesFromTexture(const QPixmap &texture, int sprite_width, int sprite_height, int separator_width = 0);

    };
}

#endif // PIXMAPTOOLS_H
