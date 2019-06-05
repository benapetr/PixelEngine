//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef RADIAN1_H
#define RADIAN1_H

#include <PixelEngine/actor.h>

class Radian1 : public PE::Actor
{
    public:
        Radian1(const PE::Vector &position);
        void Update(qint64 time = 0);
        void Render(PE::Renderer *r, PE::Camera *c) override;
        double radian = 0;
        double distance = 10;
        PE::Vector v1;
        PE::Vector v2;
};

#endif // RADIAN1_H
