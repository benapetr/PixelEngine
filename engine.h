// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
            static void Initialize();
            static Engine *GetEngine();
            static QString GetVersion();

            Engine();
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
