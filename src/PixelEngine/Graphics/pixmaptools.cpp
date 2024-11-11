//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "pixmaptools.h"
#include <QImage>
#include <QPainter>

QPixmap PE::PixmapTools::Rotate(const QPixmap &pm, qreal angle)
{
#ifdef QT6_BUILD
    QPixmap result(pm);
    QTransform transform;
    transform.rotate(angle);
    
    int pxw = result.width();
    int pxh = result.height();
    
    result = result.transformed(transform, Qt::SmoothTransformation);
    result = result.copy((result.width() - pxw) / 2, (result.height() - pxh) / 2, pxw, pxh);
    
    return result;
#else
    QPixmap result(pm);
    QMatrix rm;
    rm.rotate(angle);
    int pxw = result.width(), pxh = result.height();
    result = result.transformed(rm);
    result = result.copy((result.width() - pxw)/2, (result.height() - pxh)/2, pxw, pxh);
    return result;
#endif
}

QVector<QPixmap> PE::PixmapTools::LoadTransparentSpritesFromTexture(const QPixmap &texture, int sprite_width, int sprite_height, QColor transparent, int separator_width)
{
    QVector<QPixmap> sprites;
    int columns = texture.width() / (sprite_width + separator_width);
    int rows = texture.height() / (sprite_height + separator_width);
    QImage sprite_sheet = texture.toImage();

    // Loop through each sprite in the grid
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            QRect spriteRect;
            {
                int x, y;
                x = (col * (sprite_width + separator_width)) + separator_width;
                y = (row * (sprite_height + separator_width)) + separator_width;
                spriteRect = QRect(col * sprite_width, row * sprite_height, sprite_width, sprite_height);
            }
            QImage spriteImage = sprite_sheet.copy(spriteRect);

            for (int y = 0; y < sprite_height; ++y)
            {
                for (int x = 0; x < sprite_width; ++x)
                {
                    if (spriteImage.pixelColor(x, y) == transparent)
                    {
                        spriteImage.setPixelColor(x, y, QColor(0, 0, 0, 0)); // Set to transparent
                    }
                }
            }

            // Convert to QPixmap and store in the vector
            sprites.append(QPixmap::fromImage(spriteImage));
        }
    }

    return sprites;
}

QVector<QPixmap> PE::PixmapTools::LoadSpritesFromTexture(const QPixmap &texture, int sprite_width, int sprite_height, int separator_width)
{
    QVector<QPixmap> sprites;
    int columns = texture.width() / (sprite_width + separator_width);
    int rows = texture.height() / (sprite_height + separator_width);
    QImage sprite_sheet = texture.toImage();

    // Loop through each sprite in the grid
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < columns; ++col)
        {
            int x, y;
            x = (col * (sprite_width + separator_width)) + separator_width;
            y = (row * (sprite_height + separator_width)) + separator_width;
            QRect spriteRect(x, y, sprite_width, sprite_height);
            QImage spriteImage = sprite_sheet.copy(spriteRect);

            // Convert to QPixmap and store in the vector
            sprites.append(QPixmap::fromImage(spriteImage));
        }
    }

    return sprites;
}
