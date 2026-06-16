//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "capsulecollider.h"
#include "bitmapcollider.h"
#include "boxcollider.h"
#include "circlecollider.h"
#include "collidermath.h"
#include "ellipticcollider.h"
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

namespace
{
    pe_float_t distancePointToSegment(Vector point, Vector a, Vector b)
    {
        pe_float_t dx = b.X - a.X;
        pe_float_t dy = b.Y - a.Y;
        pe_float_t length_squared = (dx * dx) + (dy * dy);
        if (length_squared <= static_cast<pe_float_t>(0.0001))
            return point.DistanceTo(a);

        pe_float_t t = ((point.X - a.X) * dx + (point.Y - a.Y) * dy) / length_squared;
        t = std::max(static_cast<pe_float_t>(0), std::min(static_cast<pe_float_t>(1), t));
        Vector closest(a.X + (dx * t), a.Y + (dy * t));
        return point.DistanceTo(closest);
    }
}

CapsuleCollider::CapsuleCollider(pe_float_t x, pe_float_t y, pe_float_t width, pe_float_t height, Object *parent, unsigned int layer) : Collider(parent)
{
    this->Position = Vector(x, y);
    this->RelativePosition = this->Position;
    this->Width = width;
    this->Height = height;
    this->Layer = layer;
}

QString CapsuleCollider::GetClassName() const
{
    return "PE::CapsuleCollider";
}

void CapsuleCollider::Serialize(Serializer *serializer) const
{
    Collider::Serialize(serializer);
    serializer->WriteFloat("width", this->Width);
    serializer->WriteFloat("height", this->Height);
}

void CapsuleCollider::Deserialize(Deserializer *deserializer)
{
    Collider::Deserialize(deserializer);
    this->Width = deserializer->ReadFloat("width", this->Width);
    this->Height = deserializer->ReadFloat("height", this->Height);
}

Vector CapsuleCollider::Center() const
{
    return Vector(this->Position.X + (this->Width * this->Scale / 2),
                  this->Position.Y + (this->Height * this->Scale / 2));
}

Vector CapsuleCollider::LocalToWorld(Vector local) const
{
    if (!this->IsOriented())
        return local;

    Vector center = this->Center();
    pe_float_t angle = this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = local.X - center.X;
    pe_float_t y = local.Y - center.Y;
    return Vector(center.X + (x * cos_a) - (y * sin_a),
                  center.Y + (x * sin_a) + (y * cos_a));
}

Vector CapsuleCollider::WorldToLocal(Vector world) const
{
    if (!this->IsOriented())
        return world;

    Vector center = this->Center();
    pe_float_t angle = -this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = world.X - center.X;
    pe_float_t y = world.Y - center.Y;
    return Vector(center.X + (x * cos_a) - (y * sin_a),
                  center.Y + (x * sin_a) + (y * cos_a));
}

pe_float_t CapsuleCollider::GetRadius() const
{
    return std::min(this->Width * this->Scale, this->Height * this->Scale) / 2;
}

Vector CapsuleCollider::SegmentA() const
{
    pe_float_t radius = this->GetRadius();
    pe_float_t width = this->Width * this->Scale;
    pe_float_t height = this->Height * this->Scale;

    if (height >= width)
        return this->LocalToWorld(Vector(this->Position.X + (width / 2), this->Position.Y + radius));

    return this->LocalToWorld(Vector(this->Position.X + radius, this->Position.Y + (height / 2)));
}

Vector CapsuleCollider::SegmentB() const
{
    pe_float_t radius = this->GetRadius();
    pe_float_t width = this->Width * this->Scale;
    pe_float_t height = this->Height * this->Scale;

    if (height >= width)
        return this->LocalToWorld(Vector(this->Position.X + (width / 2), this->Position.Y + height - radius));

    return this->LocalToWorld(Vector(this->Position.X + width - radius, this->Position.Y + (height / 2)));
}

bool CapsuleCollider::PositionMatch(Vector position)
{
    return distancePointToSegment(position, this->SegmentA(), this->SegmentB()) <= this->GetRadius();
}

bool CapsuleCollider::IntersectionMatch(Collider *collider)
{
    switch (collider->GetColliderType())
    {
        case PE_ColliderType_Pixel:
            return this->PositionMatch(collider->Position);
        case PE_ColliderType_Box:
            return ColliderMath::IntersectionCheckBoxCapsule(dynamic_cast<BoxCollider*>(collider), this);
        case PE_ColliderType_Bitmap:
            return ColliderMath::IntersectionCheckCapsuleBitmap(this, dynamic_cast<BitmapCollider*>(collider));
        case PE_ColliderType_Circle:
            return ColliderMath::IntersectionCheckCapsuleCircle(this, dynamic_cast<CircleCollider*>(collider));
        case PE_ColliderType_Capsule:
            return ColliderMath::IntersectionCheckCapsuleCapsule(this, dynamic_cast<CapsuleCollider*>(collider));
        case PE_ColliderType_Ellipse:
            return ColliderMath::IntersectionCheckEllipseCapsule(dynamic_cast<EllipticCollider*>(collider), this);
        case PE_ColliderType_Line:
            return ColliderMath::IntersectionCheckLineCapsule(dynamic_cast<LineCollider*>(collider), this);
        case PE_ColliderType_Polygon:
            return ColliderMath::IntersectionCheckPolygonCapsule(dynamic_cast<PolygonCollider*>(collider), this);
    }

    return this->PositionMatch(collider->Position);
}

#ifdef PE_DEBUG
void CapsuleCollider::Render(Renderer *r, Camera *c)
{
    if (!Collider::Debug)
        return;

    Vector a = c->ProjectedPosition(this->SegmentA());
    Vector b = c->ProjectedPosition(this->SegmentB());
    int radius = static_cast<int>(this->GetRadius());
    r->DrawLine(a, b, radius * 2, Qt::green);
    r->DrawEllipse(a.X2int() - radius, a.Y2int() - radius, radius * 2, radius * 2, Qt::green);
    r->DrawEllipse(b.X2int() - radius, b.Y2int() - radius, radius * 2, radius * 2, Qt::green);
}
#endif
