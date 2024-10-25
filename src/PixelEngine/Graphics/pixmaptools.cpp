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
