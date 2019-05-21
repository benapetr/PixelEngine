// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef COLLECTABLE_H
#define COLLECTABLE_H

#include "../definitions.h"
#include <QList>
#include <QMutex>

namespace PE
{
    class GC;

    //! Object that is managed by a garbage collector, you should never directly call delete on these
    class Collectable
    {
        public:
            static int GetCollectablesCount();
            static QList<Collectable*> Collectables;
            static QMutex *CollectablesLock;
            Collectable();
            virtual ~Collectable();
            void IncRef();
            void DecRef();

        private:
            void collectable_Register();
            bool collectable_isRegistered = false;
            //! Number of references pointing to this collectable
            unsigned int _refCount = 0;
            // For fast memory access
            friend class GC;
    };
}

#endif // COLLECTABLE_H
