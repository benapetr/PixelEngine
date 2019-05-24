// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "pixmaptools.h"
#include <QImage>
#include <QPainter>

QPixmap PE::PixmapTools::Rotate(const QPixmap &pm, qreal angle)
{
    QPixmap result(pm);
    QMatrix rm;
    rm.rotate(angle);
    int pxw = result.width(), pxh = result.height();
    result = result.transformed(rm);
    result = result.copy((result.width() - pxw)/2, (result.height() - pxh)/2, pxw, pxh);
    return result;
}
