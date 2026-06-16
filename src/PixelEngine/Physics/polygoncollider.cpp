//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "polygoncollider.h"
#include "bitmapcollider.h"
#include "boxcollider.h"
#include "capsulecollider.h"
#include "circlecollider.h"
#include "collidermath.h"
#include "ellipticcollider.h"
#include "linecollider.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include <QVariantList>
#include <cmath>
#ifdef PE_DEBUG
#include "../camera.h"
#include "../Graphics/renderer.h"
#endif

using namespace PE;

PolygonCollider::PolygonCollider(const QList<Vector> &points, Object *parent, unsigned int layer) : Collider(parent)
{
    this->Points = points;
    this->Layer = layer;
    if (!points.isEmpty())
    {
        this->Position = points.first();
        this->RelativePosition = this->Position;
    }
}

QString PolygonCollider::GetClassName() const
{
    return "PE::PolygonCollider";
}

void PolygonCollider::Serialize(Serializer *serializer) const
{
    Collider::Serialize(serializer);
    QVariantList points;
    for (const Vector &point : this->Points)
    {
        QVariantList pair;
        pair.append(point.X);
        pair.append(point.Y);
        points.append(pair);
    }
    serializer->WriteValue("points", points);
}

void PolygonCollider::Deserialize(Deserializer *deserializer)
{
    Collider::Deserialize(deserializer);
    this->Points.clear();
    QVariantList points = deserializer->ReadValue("points").toList();
    for (const QVariant &pointValue : points)
    {
        QVariantList pair = pointValue.toList();
        if (pair.size() >= 2)
            this->Points.append(Vector(pair.at(0).toDouble(), pair.at(1).toDouble()));
    }
    if (!this->Points.isEmpty())
        this->Position = this->Points.first();
}

Vector PolygonCollider::LocalToWorld(Vector local) const
{
    Vector translated = local;
    if (this->GetParent())
        translated = this->GetParent()->GetPosition() + local;

    if (!this->IsOriented())
        return translated;

    Vector center = this->GetParent() ? this->GetParent()->GetPosition() : this->Position;
    pe_float_t angle = this->GetWorldRotation() * PE_DEG_RAD_CNV;
    pe_float_t cos_a = std::cos(angle);
    pe_float_t sin_a = std::sin(angle);
    pe_float_t x = translated.X - center.X;
    pe_float_t y = translated.Y - center.Y;
    return Vector(center.X + (x * cos_a) - (y * sin_a),
                  center.Y + (x * sin_a) + (y * cos_a));
}

Vector PolygonCollider::WorldToLocal(Vector world) const
{
    (void)world;
    return world;
}

QList<Vector> PolygonCollider::GetWorldPoints() const
{
    QList<Vector> points;
    for (const Vector &point : this->Points)
        points.append(this->LocalToWorld(point));
    return points;
}

bool PolygonCollider::PositionMatch(Vector position)
{
    QList<Vector> points = this->GetWorldPoints();
    bool inside = false;
    for (int i = 0, j = points.size() - 1; i < points.size(); j = i++)
    {
        if (((points[i].Y > position.Y) != (points[j].Y > position.Y)) &&
            (position.X < (points[j].X - points[i].X) * (position.Y - points[i].Y) / (points[j].Y - points[i].Y) + points[i].X))
        {
            inside = !inside;
        }
    }
    return inside;
}

bool PolygonCollider::IntersectionMatch(Collider *collider)
{
    switch (collider->GetColliderType())
    {
        case PE_ColliderType_Pixel:
            return this->PositionMatch(collider->Position);
        case PE_ColliderType_Box:
            return ColliderMath::IntersectionCheckPolygonBox(this, dynamic_cast<BoxCollider*>(collider));
        case PE_ColliderType_Bitmap:
            return ColliderMath::IntersectionCheckPolygonBitmap(this, dynamic_cast<BitmapCollider*>(collider));
        case PE_ColliderType_Circle:
            return ColliderMath::IntersectionCheckPolygonCircle(this, dynamic_cast<CircleCollider*>(collider));
        case PE_ColliderType_Capsule:
            return ColliderMath::IntersectionCheckPolygonCapsule(this, dynamic_cast<CapsuleCollider*>(collider));
        case PE_ColliderType_Ellipse:
            return ColliderMath::IntersectionCheckPolygonEllipse(this, dynamic_cast<EllipticCollider*>(collider));
        case PE_ColliderType_Line:
            return ColliderMath::IntersectionCheckPolygonLine(this, dynamic_cast<LineCollider*>(collider));
        case PE_ColliderType_Polygon:
            return ColliderMath::IntersectionCheckPolygonPolygon(this, dynamic_cast<PolygonCollider*>(collider));
    }

    return this->PositionMatch(collider->Position);
}

#ifdef PE_DEBUG
void PolygonCollider::Render(Renderer *r, Camera *c)
{
    if (!Collider::Debug)
        return;

    QList<Vector> points = this->GetWorldPoints();
    for (int i = 0; i < points.size(); i++)
        r->DrawLine(c->ProjectedPosition(points[i]), c->ProjectedPosition(points[(i + 1) % points.size()]), 1, Qt::green);
}
#endif
