//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef P1_H
#define P1_H

#include <PixelEngine/actor.h>

class P1 : public PE::Actor
{
    public:
        P1(const PE::Vector &position);
        void Render(PE::Renderer *r, PE::Camera *c) override;
};

#endif // P1_H
