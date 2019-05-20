// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "gc_thread.h"
#include "gc.h"
#include "../engine.h"

using namespace PE;

GC_thread::GC_thread()
{

}

void GC_thread::Stop()
{
    this->running = false;
}

bool GC_thread::IsStopped() const
{
    return this->stopped;
}

bool GC_thread::IsRunning() const
{
    return this->running;
}

void GC_thread::run()
{
    while (this->running)
    {
        if (!Engine::GetEngine()->GetGC()->Collect())
            GC_thread::usleep(20000);
        GC_thread::usleep(2000);
    }

    this->stopped = true;
}
