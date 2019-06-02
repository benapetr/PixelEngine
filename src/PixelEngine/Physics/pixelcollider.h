//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef PIXELCOLLIDER_H
#define PIXELCOLLIDER_H

#include "collider.h"

namespace PE
{
    class PixelCollider : public Collider
    {
        public:
            PixelCollider(double x, double y);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            PE_ColliderType GetColliderType() override;
#ifdef PE_DEBUG
            void Render(PE::Renderer *r, PE::Camera *c) override;
#endif
    };
}

#endif // PIXELCOLLIDER_H
