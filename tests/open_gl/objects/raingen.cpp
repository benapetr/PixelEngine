//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "raingen.h"
#include "../game.h"
#include "waterdrop.h"
#include <PixelEngine/world.h>
#include <PixelEngine/pemath.h>

RainGen::RainGen()
{

}

void RainGen::Update(qint64 time)
{
    if (time - this->last_time < 20)
        return;
    this->last_time = time;
    Game::CurrentGame->GetWorld()->RegisterActor(new WaterDrop(PE::Vector(PE::PEMath::GetRandom(0, 1600), 1000)));
}
