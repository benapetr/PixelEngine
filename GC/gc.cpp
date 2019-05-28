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
