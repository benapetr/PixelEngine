//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "collectable.h"
#include "../exception.h"
#include <assert.h>

using namespace PE;

QMutex *Collectable::CollectablesLock = new QMutex(QMutex::Recursive);
QList<Collectable*> Collectable::Collectables;

int Collectable::GetCollectablesCount()
{
    int result;
    Collectable::CollectablesLock->lock();
    result = Collectable::Collectables.size();
    Collectable::CollectablesLock->unlock();
    return result;
}

Collectable::Collectable()
{
    Collectable::CollectablesLock->lock();
    Collectable::Collectables.append(this);
    Collectable::CollectablesLock->unlock();
}

Collectable::~Collectable()
{
    //if (this->_refCount > 0)
    //    throw new Exception("Managed resource with references removal", BOOST_CURRENT_FUNCTION);

    assert(this->_refCount == 0);

    Collectable::CollectablesLock->lock();
#ifdef PE_DEBUG
    if (!Collectable::Collectables.removeOne(this))
    {
        Collectable::CollectablesLock->unlock();
        // This is for debugging only
        throw new Exception("Collectable was not registered", BOOST_CURRENT_FUNCTION);
    }
#else
    Collectable::Collectables.removeOne(this);
#endif
    Collectable::CollectablesLock->unlock();
}

void Collectable::IncRef()
{
    this->_refCount++;
}

void Collectable::DecRef()
{
    if (this->_refCount == 0)
        throw new Exception("Negative reference count", BOOST_CURRENT_FUNCTION);
    this->_refCount--;
}
