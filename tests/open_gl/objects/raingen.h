//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef RAINGEN_H
#define RAINGEN_H

#include <PixelEngine/actor.h>

class RainGen : public PE::Actor
{
    public:
        RainGen();
        void Update(qint64 time = 0) override;
    private:
        qint64 last_time = 0;
};

#endif // RAINGEN_H
