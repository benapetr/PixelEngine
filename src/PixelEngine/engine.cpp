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
#include "GC/gc.h"
#include "pemath.h"
#include "nullpointerexception.h"
#include "ringlog.h"
#include "object.h"
#include "actor.h"
#include "Physics/boxcollider.h"
#include "Physics/circlecollider.h"
#include "Physics/pixelcollider.h"
#include "Physics/rigidbody.h"
#include "Resources/assetcontainer.h"
#include "Serialization/classregistry.h"

using namespace PE;

Engine *Engine::engine = nullptr;

void Engine::Initialize(bool multithreaded_gc)
{
    if (Engine::engine)
        return;

    Engine::engine = new Engine(multithreaded_gc);
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

Engine::Engine(bool multithreaded_gc)
{
    this->startupTime = QDateTime::currentDateTime();
    this->RL = new RingLog();
    ClassRegistry::GetRegistry()->RegisterClass("PE::Object", []() { return new Object(); });
    ClassRegistry::GetRegistry()->RegisterClass("PE::Actor", []() { return new Actor(); });
    ClassRegistry::GetRegistry()->RegisterClass("PE::Rigidbody", []() { return new Rigidbody(); });
    ClassRegistry::GetRegistry()->RegisterClass("PE::BoxCollider", []() { return new BoxCollider(0, 0, 0, 0); });
    ClassRegistry::GetRegistry()->RegisterClass("PE::CircleCollider", []() { return new CircleCollider(0, 0, 0); });
    ClassRegistry::GetRegistry()->RegisterClass("PE::PixelCollider", []() { return new PixelCollider(0, 0); });
    ClassRegistry::GetRegistry()->RegisterClass("PE::AssetContainer", []() { return new AssetContainer(); });
    this->gc = new GC(multithreaded_gc);
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
