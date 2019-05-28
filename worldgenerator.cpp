//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019
#include "worldgenerator.h"
#include "Physics/bitmapcollider.h"
#include "pemath.h"
#include <QImage>
#include <QPen>
#include <QBitmap>
#include <QPainter>
#include <QBitmap>

using namespace PE;

Collectable_SmartPtr<Terrain> WorldGenerator::GenerateRandom(int w_width, int w_height)
{
    Collectable_SmartPtr<Terrain> w = new Terrain(0, 0, w_width, w_height);
    w->TerrainColor = Qt::black;
    w->BackgroundColor = QColor(178, 207, 255);
    QImage image(w_width, w_height, QImage::Format_RGB32);
    image.fill(w->TerrainColor);
    QPainter painter(&image);
    QPen pen(w->BackgroundColor);
    painter.setPen(pen);

    // let's generate the terrain
    int STEP_MAX = 8;
    int STEP_CHANGE = 2;
    int HEIGHT_MIN = 160;

    // starting conditions
    int height = PEMath::GetRandom(0, w_height);
    int slope = PEMath::GetRandom(-8, 8);

    // creating the landscape
    for (int x = 0; x < w_width; x++)
    {
        // change height and slope
        height += slope;
        slope += PEMath::GetRandom(-7, 8);

        // clip height and slope to maximum
        if (slope > STEP_MAX)
            slope = STEP_MAX;
        if (slope < -STEP_MAX)
            slope = -STEP_MAX;

        if (height < HEIGHT_MIN)
        {
            height = HEIGHT_MIN;
            slope *= -1;
        }
        if (height < 0)
        {
            height = 0;
            slope *= -1;
        }

        // draw column
        painter.drawLine(x, 0, x, height);

        int real_height = w_height - height;
        while (real_height > 0)
        {
            w->Collider->Bitmap[x][real_height--] = true;
        }
    }
    w->SourceImage = image;
    w->BitMap = QPixmap::fromImage(image);

    return w;
}
