// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "pemath.h"
#include <QDateTime>
#include <QRandomGenerator>

using namespace PE;

qint32 PEMath::GetRandom(qint32 min, qint32 max)
{
    return QRandomGenerator::global()->bounded(min, max);
}
