//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "linecollider.h"
#include "bitmapcollider.h"
#include "boxcollider.h"
#include "capsulecollider.h"
#include "circlecollider.h"
#include "collidermath.h"
#include "ellipticcollider.h"
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
    pe_float_t pointSegmentDistance(Vector point, Vector a, Vector b)
    {
        pe_float_t dx = b.X - a.X;
        pe_float_t dy = b.Y - a.Y;
        pe_float_t length_squared = (dx * dx) + (dy * dy);
        if (length_squared <= static_cast<pe_float_t>(0.0001))
            return point.DistanceTo(a);

        pe_float_t t = ((point.X - a.X) * dx + (point.Y - a.Y) * dy) / length_squared;
        t = std::max(static_cast<pe_float_t>(0), std::min(static_cast<pe_float_t>(1), t));
        return point.DistanceTo(Vector(a.X + (dx * t), a.Y + (dy * t)));
    }
}

LineCollider::LineCollider(Vector start, Vector end, pe_float_t thickness, Object *parent, unsigned int layer) : Collider(parent)
{
    this->LocalStart = start;
    this->LocalEnd = end;
    this->Position = start;
    this->RelativePosition = start;
    this->Thickness = thickness;
    this->Layer = layer;
}

QString LineCollider::GetClassName() const
{
    return "PE::LineCollider";
}

void LineCollider::Serialize(Serializer *serializer) const
{
    Collider::Serialize(serializer);
    serializer->WriteVector("start", this->LocalStart);
    serializer->WriteVector("end", this->LocalEnd);
    serializer->WriteFloat("thickness", this->Thickness);
}

void LineCollider::Deserialize(Deserializer *deserializer)
{
    Collider::Deserialize(deserializer);
    this->LocalStart = deserializer->ReadVector("start", this->LocalStart);
    this->LocalEnd = deserializer->ReadVector("end", this->LocalEnd);
    this->Thickness = deserializer->ReadFloat("thickness", this->Thickness);
    this->Position = this->LocalStart;
    this->RelativePosition = this->LocalStart;
}

Vector LineCollider::LocalToWorld(Vector local) const
{
    Vector translated = local;
    if (this->GetParent())
        translated = this->GetParent()->GetPosition() + local;

    if (!this->IsOriented())
        return translated;

    Vector center = this->Start();
    pe_float_t angle = this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = translated.X - center.X;
    pe_float_t y = translated.Y - center.Y;
    return Vector(center.X + (x * cos_a) - (y * sin_a),
                  center.Y + (x * sin_a) + (y * cos_a));
}

Vector LineCollider::WorldToLocal(Vector world) const
{
    (void)world;
    return world;
}

Vector LineCollider::Start() const
{
    if (this->GetParent())
        return this->GetParent()->GetPosition() + this->LocalStart;
    return this->LocalStart;
}

Vector LineCollider::End() const
{
    if (!this->IsOriented())
    {
        if (this->GetParent())
            return this->GetParent()->GetPosition() + this->LocalEnd;
        return this->LocalEnd;
    }

    Vector start = this->Start();
    Vector end = this->GetParent() ? this->GetParent()->GetPosition() + this->LocalEnd : this->LocalEnd;
    pe_float_t angle = this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = end.X - start.X;
    pe_float_t y = end.Y - start.Y;
    return Vector(start.X + (x * cos_a) - (y * sin_a),
                  start.Y + (x * sin_a) + (y * cos_a));
}

bool LineCollider::PositionMatch(Vector position)
{
    return pointSegmentDistance(position, this->Start(), this->End()) <= (this->Thickness * this->Scale / 2);
}

bool LineCollider::IntersectionMatch(Collider *collider)
{
    switch (collider->GetColliderType())
    {
        case PE_ColliderType_Pixel:
            return this->PositionMatch(collider->Position);
        case PE_ColliderType_Box:
            return ColliderMath::IntersectionCheckLineBox(this, dynamic_cast<BoxCollider*>(collider));
        case PE_ColliderType_Bitmap:
            return ColliderMath::IntersectionCheckLineBitmap(this, dynamic_cast<BitmapCollider*>(collider));
        case PE_ColliderType_Circle:
            return ColliderMath::IntersectionCheckLineCircle(this, dynamic_cast<CircleCollider*>(collider));
        case PE_ColliderType_Capsule:
            return ColliderMath::IntersectionCheckLineCapsule(this, dynamic_cast<CapsuleCollider*>(collider));
        case PE_ColliderType_Ellipse:
            return ColliderMath::IntersectionCheckLineEllipse(this, dynamic_cast<EllipticCollider*>(collider));
        case PE_ColliderType_Line:
            return ColliderMath::IntersectionCheckLineLine(this, dynamic_cast<LineCollider*>(collider));
        case PE_ColliderType_Polygon:
            return ColliderMath::IntersectionCheckPolygonLine(dynamic_cast<PolygonCollider*>(collider), this);
    }

    return this->PositionMatch(collider->Position);
}

#ifdef PE_DEBUG
void LineCollider::Render(Renderer *r, Camera *c)
{
    if (!Collider::Debug)
        return;

    r->DrawLine(c->ProjectedPosition(this->Start()), c->ProjectedPosition(this->End()), static_cast<int>(this->Thickness * this->Scale), Qt::green);
}
#endif
