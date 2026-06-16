//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef POLYGONCOLLIDER_H
#define POLYGONCOLLIDER_H

#include "../definitions.h"
#include "collider.h"
#include <QList>

namespace PE
{
    class PolygonCollider : public Collider
    {
        public:
            PolygonCollider(const QList<Vector> &points = QList<Vector>(), Object *parent = nullptr, unsigned int layer = 0);
            bool PositionMatch(Vector position) override;
            bool IntersectionMatch(Collider *collider) override;
            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            PE_ColliderType GetColliderType() override { return PE_ColliderType_Polygon; }
            QList<Vector> GetWorldPoints() const;
            Vector LocalToWorld(Vector local) const;
            Vector WorldToLocal(Vector world) const;
#ifdef PE_DEBUG
            void Render(PE::Renderer *r, PE::Camera *c) override;
#endif
            QList<Vector> Points;
    };
}

#endif // POLYGONCOLLIDER_H
