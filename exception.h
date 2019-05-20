// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>

namespace PE
{
    class Exception
    {
        public:
            Exception(const QString &what, const QString &where);
            virtual ~Exception();
            // What happened
            QString What;
            QString Where;
    };
}

#endif // EXCEPTION_H
