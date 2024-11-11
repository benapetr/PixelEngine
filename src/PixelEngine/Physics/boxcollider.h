//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "../definitions.h"
#include "collider.h"

namespace PE
{
    class ColliderMath;
    class BoxCollider : public Collider
    {
        public:
            BoxCollider(pe_float_t x, pe_float_t y, pe_float_t w, pe_float_t h, Object *parent = nullptr, unsigned int layer = 0);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            pe_float_t GetX() const { return this->Position.X; }
            pe_float_t GetY() const { return this->Position.Y; }
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Box; }
#ifdef PE_DEBUG
            void Render(PE::Renderer *r, PE::Camera *c) override;
#endif
            /*
             * D +--------+ C
             *   |        |
             *   |        |
             * A +--------+ B
             * */
            inline Vector A();
            inline Vector B();
            inline Vector C();
            inline Vector D();
            pe_float_t Width;
            pe_float_t Height;

        private:
            bool boxMatch(BoxCollider *c);

            friend class ColliderMath;
    };

    inline Vector BoxCollider::A()
    {
        return this->Position;
    }

    inline Vector BoxCollider::B()
    {
        Vector b(this->Position);
        b.X += this->Width * this->Scale;
        return b;
    }

    inline Vector BoxCollider::C()
    {
        Vector c(this->Position);
        c.X += this->Width * this->Scale;
        c.Y += this->Height * this->Scale;
        return c;
    }

    inline Vector BoxCollider::D()
    {
        Vector d(this->Position);
        d.Y += this->Height * this->Scale;
        return d;
    }
}

#endif // BOXCOLLIDER_H
