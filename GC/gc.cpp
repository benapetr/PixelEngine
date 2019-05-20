// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "collectable.h"
#include "gc.h"
#include "gc_thread.h"

using namespace PE;

GC::GC()
{
    this->thread = new GC_thread();
}

GC::~GC()
{
    delete this->thread;
}

void GC::Start()
{
    this->thread->start();
}

void GC::Stop()
{
    this->thread->Stop();
    while (!this->thread->IsStopped())
        QThread::sleep(200);
}

int GC::Collect()
{
    int items = 0;
    // Copy this list to prevent unnecessary locks
    Collectable::CollectablesLock->lock();
    QList<Collectable*> collectables(Collectable::Collectables);
    Collectable::CollectablesLock->unlock();
    foreach (Collectable *c, collectables)
    {
        if (c->_refCount == 0)
        {
            items++;
            delete c;
        }
    }
    return items;
}
