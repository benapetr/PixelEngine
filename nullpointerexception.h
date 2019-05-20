// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef NULLPOINTEREXCEPTION_H
#define NULLPOINTEREXCEPTION_H

#include "exception.h"

namespace PE
{
    class NullPointerException : public Exception
    {
        public:
            NullPointerException(const QString &what, const QString &where);
    };
}

#endif // NULLPOINTEREXCEPTION_H
