//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef ENGINE_H
#define ENGINE_H

#include "definitions.h"
#include <QDateTime>

#define PE_WRITE_RING(text) { PE::Engine::GetEngine()->RL->WriteText(text); }

namespace PE
{
    class RingLog;
    class GC;

    class Engine
    {
        public:
            static void Initialize(bool multithreaded_gc = true);
            static Engine *GetEngine();
            static QString GetVersion();

            Engine(bool multithreaded_gc = true);
            virtual ~Engine();
            RingLog *RL = nullptr;
            GC *GetGC() const;

        private:
            static Engine *engine;
            GC *gc;
            QDateTime startupTime;
    };
}

#endif // ENGINE_H
