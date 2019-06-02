//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "game.h"
#include "objects/c1.h"
#include "objects/r1.h"
#include "objects/p1.h"
#include <PixelEngine/engine.h>
#include <PixelEngine/GC/gc.h>
#include <PixelEngine/Graphics/renderer.h>
#include <PixelEngine/pemath.h>
#include <PixelEngine/Physics/boxcollider.h>
#include <PixelEngine/world.h>
#include <PixelEngine/worldgenerator.h>
#include "console.h"

Game *Game::CurrentGame = nullptr;
bool  Game::SuperFast = false;
bool  Game::Tracing = false;
bool  Game::FastTerrainUpdates = true;

Game::Game(double w_width, double w_height, PE::Renderer *r)
{
    Game::CurrentGame = this;
    this->renderer = r;
    this->MapHeight = w_height;
    this->MapWidth = w_width;
    this->renderer->ManualUpdate = true;

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(OnUpdate()));
    this->timer->start(20);
    this->startGame();

    PE::Collider::Debug = true;
}

Game::~Game()
{
    this->timer->stop();
    delete this->world;
}

void Game::startGame()
{
    this->resetWorld();
    QColor background_color = this->getRandomBackgroundColor();
    QColor terrain = this->getRandomTerrainColor();
    this->Terrain = PE::WorldGenerator::GenerateRandom(static_cast<int>(this->MapWidth), static_cast<int>(this->MapHeight) - 20, background_color, terrain);
    // Move the terrain little bit higher, so there is space for HUD
    PE::Vector terrain_position = this->Terrain->Position;
    terrain_position.Y += 20;
    this->Terrain->SetPosition(terrain_position);
    this->world->RegisterTerrain(this->Terrain);
    this->world->BorderColor = QColor("blue");

    // Create floor
    this->world->RegisterCollider(new PE::BoxCollider(-1000, -100, 4000, 180));
    this->world->RegisterCollider(new PE::BoxCollider(-100, -100, 100, 2000));
    this->world->RegisterCollider(new PE::BoxCollider(this->MapWidth, -100, 100, 2000));
    this->world->RegisterCollider(new PE::BoxCollider(-1000, this->MapHeight + 800, 4000, 100));

    this->world->RegisterActor(new C1(PE::Vector(80, this->MapHeight - 100)));
    this->world->RegisterActor(new C1(PE::Vector(85, this->MapHeight - 100 + 20)));
    this->world->RegisterActor(new C1(PE::Vector(86, this->MapHeight - 100 + 50)));
    this->world->RegisterActor(new C1(PE::Vector(96, this->MapHeight - 100 + 80)));
    this->world->RegisterActor(new C1(PE::Vector(92, this->MapHeight - 100 + 120)));
    this->world->RegisterActor(new C1(PE::Vector(94, this->MapHeight - 100 + 150)));
    this->world->RegisterActor(new R1(PE::Vector(96, this->MapHeight - 300 + 180)));

    int x = 120;

    while (x < this->MapWidth)
    {
        x += 32;
        this->world->RegisterActor(new C1(PE::Vector(x, this->MapHeight - 100)));
        this->world->RegisterActor(new R1(PE::Vector(x, this->MapHeight - 100 + 80)));
    }
}

void Game::test1()
{

}

void Game::test2()
{
    this->resetWorld();
    this->world->BorderColor = QColor(Qt::magenta);

    // Create floor
    this->world->RegisterCollider(new PE::BoxCollider(-1000, -100, 4000, 180));
    this->world->RegisterCollider(new PE::BoxCollider(-100, -100, 100, 2000));
    this->world->RegisterCollider(new PE::BoxCollider(this->MapWidth, -100, 100, 2000));
    this->world->RegisterCollider(new PE::BoxCollider(-1000, this->MapHeight + 800, 4000, 100));

    this->world->RegisterActor(new C1(PE::Vector(80, this->MapHeight - 20)));
    this->world->RegisterActor(new P1(PE::Vector(80, this->MapHeight)));
    //this->world->RegisterActor(new P1(PE::Vector(72, this->MapHeight - 60)));
    //this->world->RegisterActor(new P1(PE::Vector(90, this->MapHeight - 60)));

    this->world->RegisterActor(new C1(PE::Vector(120, this->MapHeight - 80)));
    this->world->RegisterActor(new P1(PE::Vector(118, this->MapHeight - 60)));
    this->world->RegisterActor(new P1(PE::Vector(120, this->MapHeight - 60)));
    this->world->RegisterActor(new P1(PE::Vector(122, this->MapHeight - 60)));
}

void Game::OnUpdate()
{
    // emscripten doesn't support multi-threading so we have to do this using timer
#ifdef __EMSCRIPTEN__
    PE::Engine::GetEngine()->GetGC()->Collect();
#else
    // Temporary hack - let's check if random crashes aren't caused by multithreaded GC
    PE::Engine::GetEngine()->GetGC()->Collect();
#endif


    this->updateTerrain();
    this->world->Update();

    if (Game::SuperFast)
    {
        int x = 5;
        while (--x > 0)
        {
            this->world->Update();
        }
    }
}

QColor Game::getRandomBackgroundColor()
{
    int color = PE::PEMath::GetRandom(0, 5);
    switch (color)
    {
        case 0:
            return QColor(190, 200, 216);
        case 1:
            return QColor(174, 180, 191);
        case 2:
            return QColor(185, 202, 232);
        case 3:
            return QColor(139, 175, 239);
        case 4:
            return QColor(178, 207, 255);
        case 5:
            return QColor(249, 240, 219);
    }
    return QColor(178, 207, 255);
}

QColor Game::getRandomTerrainColor()
{
    int color = PE::PEMath::GetRandom(0, 5);
    switch (color)
    {
        case 0:
            return QColor(30, 33, 31);
        case 1:
            return QColor(46, 48, 48);
        case 2:
            return QColor(35, 63, 43);
        case 3:
            return QColor(0, 0, 0);
        case 4:
            return QColor(7, 112, 37);
        case 5:
            return QColor(6, 66, 23);
    }
    return QColor(10, 10, 10);
}

void Game::updateTerrain()
{
    // Update terrain
    if (this->DynamicTerrain)
    {
        if (this->TerrainNeedsUpdate)
        {
            if (Game::FastTerrainUpdates)
                this->lastTerrainShiftedPoints = this->Terrain->ShiftFloatingBitsDown();
            else
                this->lastTerrainShiftedPoints = this->Terrain->ShiftFloatingBitsDownByOnePixel();
            this->Terrain->RefreshPixmap();
            this->Terrain->RedrawNeeded = true;
            if (this->lastTerrainShiftedPoints == 0)
            {
                // Terrain is all layed
                this->TerrainNeedsUpdate = false;
            }
        }
    }
}

void Game::resetWorld()
{
    Tracing = false;
    this->IsFinished = false;
    delete this->world;
    this->world = new PE::World(this->MapWidth, this->MapHeight);
    this->world->BackgroundColor = QColor(204, 221, 255);
}
