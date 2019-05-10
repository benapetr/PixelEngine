//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "engine.h"
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
    return Engine::engine;
}

Engine::Engine()
{
    this->startupTime = QDateTime::currentDateTime();
    this->RL = new RingLog();
    PE_WRITE_RING(QString("PixelEngine v. ") + QString(PE_VERSION_STRING) + " initialized");
}

PE::Engine::~Engine()
{
    delete this->RL;
}
