// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "collider.h"

namespace PE
{
    class ColliderMath;
    class BoxCollider : public Collider
    {
        public:
            BoxCollider(double x, double y, double w, double h);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            double GetX() const { return this->Position.X; }
            double GetY() const { return this->Position.Y; }
            double GetWidth() const { return this->width; };
            double GetHeight() const { return this->height; };
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Box; }

        private:
            bool boxMatch(BoxCollider *c);
            double width;
            double height;

            friend class ColliderMath;
    };
}

#endif // BOXCOLLIDER_H
