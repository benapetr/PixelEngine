//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
