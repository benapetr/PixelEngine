// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef GC_H
#define GC_H

#include <QList>

namespace PE
{
    class Collectable;
    class GC_thread;
    class GC
    {
        public:
            GC();
            virtual ~GC();
            void Start();
            void Stop();
            int Collect();
        private:
            GC_thread *thread;
    };
}

#endif // GC_H
