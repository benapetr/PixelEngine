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
    this->RL->WriteText(QString("PixelEngine v. ") + QString(PE_VERSION_STRING) + " initialized");
}

PE::Engine::~Engine()
{
    delete this->RL;
}
