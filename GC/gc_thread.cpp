//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
