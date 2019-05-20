// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
