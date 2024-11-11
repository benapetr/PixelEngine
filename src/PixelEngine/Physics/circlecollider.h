//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019
#ifndef CIRCLECOLLIDER_H
#define CIRCLECOLLIDER_H

#include "../definitions.h"
#include "collider.h"

namespace PE
{
    class CircleCollider : public Collider
    {
        public:
            CircleCollider(pe_float_t x, pe_float_t y, pe_float_t radius, unsigned int layer = 0);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Circle; }
#ifdef PE_DEBUG
            void Render(PE::Renderer *r, PE::Camera *c) override;
#endif
            pe_float_t Radius;
    };
}

#endif // CIRCLECOLLIDER_H
