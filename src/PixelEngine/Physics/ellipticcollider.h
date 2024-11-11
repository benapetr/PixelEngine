//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef ELLIPTICCOLLIDER_H
#define ELLIPTICCOLLIDER_H

#include "../definitions.h"
#include "collider.h"

namespace PE
{
    class EllipticCollider : public Collider
    {
        public:
            EllipticCollider(pe_float_t x, pe_float_t y, pe_float_t rx, pe_float_t ry);
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Ellipse; }
            pe_float_t RadiusX;
            pe_float_t RadiusY;
            pe_float_t Angle = 0;
    };
}

#endif // ELLIPTICCOLLIDER_H
