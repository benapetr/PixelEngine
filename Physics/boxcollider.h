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

#include "collider.h"

namespace PE
{
    class ColliderMath;
    class BoxCollider : public Collider
    {
        public:
            BoxCollider(double x, double y, double w, double h, Object *parent = nullptr);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            double GetX() const { return this->Position.X; }
            double GetY() const { return this->Position.Y; }
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Box; }
            double Width;
            double Height;

        private:
            bool boxMatch(BoxCollider *c);

            friend class ColliderMath;
    };
}

#endif // BOXCOLLIDER_H
