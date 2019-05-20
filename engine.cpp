// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "engine.h"
#include "GC/gc.h"
#include "pemath.h"
#include "nullpointerexception.h"
#include "ringlog.h"

using namespace PE;

Engine *Engine::engine = nullptr;

void Engine::Initialize()
{
    if (Engine::engine)
        return;

    Engine::engine = new Engine();
}

Engine *Engine::GetEngine()
{
    if (!Engine::engine)
        throw NullPointerException("Engine::engine", BOOST_CURRENT_FUNCTION);

    return Engine::engine;
}

QString Engine::GetVersion()
{
    return QString(PE_VERSION_STRING);
}

Engine::Engine()
{
    this->startupTime = QDateTime::currentDateTime();
    this->RL = new RingLog();
    this->gc = new GC();
    this->gc->Start();
    this->RL->WriteText(QString("PixelEngine v. ") + QString(PE_VERSION_STRING) + " initialized");
}

PE::Engine::~Engine()
{
    this->gc->Stop();
    delete this->gc;
    delete this->RL;
    Engine::engine = nullptr;
}

GC *Engine::GetGC() const
{
    return this->gc;
}
