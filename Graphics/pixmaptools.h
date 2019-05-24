// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef PIXMAPTOOLS_H
#define PIXMAPTOOLS_H

#include <QPixmap>

namespace PE
{
    class PixmapTools
    {
        public:
            //! This is slow - do cache results
            static QPixmap Rotate(const QPixmap &pm, qreal angle);
    };
}

#endif // PIXMAPTOOLS_H
