//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef CAPSULECOLLIDER_H
#define CAPSULECOLLIDER_H

#include "../definitions.h"
#include "collider.h"

namespace PE
{
    class CapsuleCollider : public Collider
    {
        public:
            CapsuleCollider(pe_float_t x, pe_float_t y, pe_float_t width, pe_float_t height, Object *parent = nullptr, unsigned int layer = 0);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Capsule; }
            Vector SegmentA() const;
            Vector SegmentB() const;
            Vector Center() const;
            Vector LocalToWorld(Vector local) const;
            Vector WorldToLocal(Vector world) const;
            pe_float_t GetRadius() const;
#ifdef PE_DEBUG
            void Render(PE::Renderer *r, PE::Camera *c) override;
#endif
            pe_float_t Width;
            pe_float_t Height;
    };
}

#endif // CAPSULECOLLIDER_H
