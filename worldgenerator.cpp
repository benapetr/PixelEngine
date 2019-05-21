// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
    QImage image(w_width, w_height, QImage::Format_RGB32);
    image.fill(Qt::black);
    QPainter painter(&image);
    QPen pen(Qt::white);
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
    w->BitMap = QBitmap(QPixmap::fromImage(image));

    return w;
}
