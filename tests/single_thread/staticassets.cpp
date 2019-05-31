//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include <PixelEngine/Graphics/pixmaptools.h>
#include <PixelEngine/resources.h>
#include "staticassets.h"

StaticAssets *StaticAssets::Instance = nullptr;

StaticAssets::StaticAssets()
{
    this->RightArrow = PE::Resources::GetPixmap(":/textures/ui/arrow.png");
    this->LeftArrow = PE::PixmapTools::Rotate(PE::Resources::GetPixmap(":/textures/ui/arrow.png"), 180);
    this->UpArrow = PE::PixmapTools::Rotate(PE::Resources::GetPixmap(":/textures/ui/arrow.png"), 90);
    this->DownArrow = PE::PixmapTools::Rotate(PE::Resources::GetPixmap(":/textures/ui/arrow.png"), 270);
}
