//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "ellipticcollider.h"
#include "bitmapcollider.h"
#include "boxcollider.h"
#include "capsulecollider.h"
#include "circlecollider.h"
#include "collidermath.h"
#include "linecollider.h"
#include "polygoncollider.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include <cmath>
#ifdef PE_DEBUG
#include "../camera.h"
#include "../Graphics/renderer.h"
#endif

using namespace PE;

EllipticCollider::EllipticCollider(pe_float_t x, pe_float_t y, pe_float_t rx, pe_float_t ry, Object *parent, unsigned int layer) : Collider(parent)
{
    this->RadiusX = rx;
    this->RadiusY = ry;
    this->Position = Vector(x, y);
    this->RelativePosition = this->Position;
    this->Layer = layer;
}

QString EllipticCollider::GetClassName() const
{
    return "PE::EllipticCollider";
}

void EllipticCollider::Serialize(Serializer *serializer) const
{
    Collider::Serialize(serializer);
    serializer->WriteFloat("radiusX", this->RadiusX);
    serializer->WriteFloat("radiusY", this->RadiusY);
}

void EllipticCollider::Deserialize(Deserializer *deserializer)
{
    Collider::Deserialize(deserializer);
    this->RadiusX = deserializer->ReadFloat("radiusX", this->RadiusX);
    this->RadiusY = deserializer->ReadFloat("radiusY", this->RadiusY);
}

Vector EllipticCollider::LocalToWorld(Vector local) const
{
    if (!this->IsOriented())
        return local;

    pe_float_t angle = this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = local.X - this->Position.X;
    pe_float_t y = local.Y - this->Position.Y;
    return Vector(this->Position.X + (x * cos_a) - (y * sin_a),
                  this->Position.Y + (x * sin_a) + (y * cos_a));
}

Vector EllipticCollider::WorldToLocal(Vector world) const
{
    if (!this->IsOriented())
        return world;

    pe_float_t angle = -this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = world.X - this->Position.X;
    pe_float_t y = world.Y - this->Position.Y;
    return Vector(this->Position.X + (x * cos_a) - (y * sin_a),
                  this->Position.Y + (x * sin_a) + (y * cos_a));
}

bool EllipticCollider::PositionMatch(Vector position)
{
    if (this->RadiusX <= 0 || this->RadiusY <= 0)
        return false;

    Vector local = this->WorldToLocal(position);
    pe_float_t rx = this->RadiusX * this->Scale;
    pe_float_t ry = this->RadiusY * this->Scale;
    pe_float_t dx = (local.X - this->Position.X) / rx;
    pe_float_t dy = (local.Y - this->Position.Y) / ry;
    return (dx * dx) + (dy * dy) <= 1;
}

bool EllipticCollider::IntersectionMatch(Collider *collider)
{
    switch (collider->GetColliderType())
    {
        case PE_ColliderType_Pixel:
            return this->PositionMatch(collider->Position);
        case PE_ColliderType_Box:
            return ColliderMath::IntersectionCheckEllipseBox(this, dynamic_cast<BoxCollider*>(collider));
        case PE_ColliderType_Bitmap:
            return collider->PositionMatch(this->Position);
        case PE_ColliderType_Circle:
            return ColliderMath::IntersectionCheckEllipseCircle(this, dynamic_cast<CircleCollider*>(collider));
        case PE_ColliderType_Capsule:
            return ColliderMath::IntersectionCheckEllipseCapsule(this, dynamic_cast<CapsuleCollider*>(collider));
        case PE_ColliderType_Ellipse:
            return ColliderMath::IntersectionCheckEllipseEllipse(this, dynamic_cast<EllipticCollider*>(collider));
        case PE_ColliderType_Line:
            return ColliderMath::IntersectionCheckLineEllipse(dynamic_cast<LineCollider*>(collider), this);
        case PE_ColliderType_Polygon:
            return ColliderMath::IntersectionCheckPolygonEllipse(dynamic_cast<PolygonCollider*>(collider), this);
    }

    return this->PositionMatch(collider->Position);
}

#ifdef PE_DEBUG
void EllipticCollider::Render(Renderer *r, Camera *c)
{
    if (!Collider::Debug)
        return;

    Vector center = c->ProjectedPosition(this->Position);
    int width = static_cast<int>(this->RadiusX * this->Scale * 2);
    int height = static_cast<int>(this->RadiusY * this->Scale * 2);
    r->DrawEllipse(center.X2int() - width / 2, center.Y2int() - height / 2, width, height, Qt::green);
}
#endif
