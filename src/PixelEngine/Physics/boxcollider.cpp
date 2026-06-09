//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "boxcollider.h"
#include "bitmapcollider.h"
#include "circlecollider.h"
#include "collidermath.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"
#include <cmath>
#ifdef PE_DEBUG
#include "../camera.h"
#include "../Graphics/renderer.h"
#endif

using namespace PE;

BoxCollider::BoxCollider(pe_float_t x, pe_float_t y, pe_float_t w, pe_float_t h, Object *parent, unsigned int layer) : Collider(parent)
{
    this->Position.X = x;
    this->Position.Y = y;
    this->RelativePosition = this->Position;
    this->Width = w;
    this->Height = h;
    this->Layer = layer;
}

QString BoxCollider::GetClassName() const
{
    return "PE::BoxCollider";
}

void BoxCollider::Serialize(Serializer *serializer) const
{
    Collider::Serialize(serializer);
    serializer->WriteFloat("width", this->Width);
    serializer->WriteFloat("height", this->Height);
}

void BoxCollider::Deserialize(Deserializer *deserializer)
{
    Collider::Deserialize(deserializer);
    this->Width = deserializer->ReadFloat("width", this->Width);
    this->Height = deserializer->ReadFloat("height", this->Height);
}

Vector BoxCollider::Center() const
{
    return Vector(this->Position.X + (this->Width * this->Scale / 2),
                  this->Position.Y + (this->Height * this->Scale / 2));
}

Vector BoxCollider::LocalToWorld(Vector local) const
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

Vector BoxCollider::WorldToLocal(Vector world) const
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

Vector BoxCollider::A()
{
    return this->LocalToWorld(this->Position);
}

Vector BoxCollider::B()
{
    return this->LocalToWorld(Vector(this->Position.X + (this->Width * this->Scale), this->Position.Y));
}

Vector BoxCollider::C()
{
    return this->LocalToWorld(Vector(this->Position.X + (this->Width * this->Scale), this->Position.Y + (this->Height * this->Scale)));
}

Vector BoxCollider::D()
{
    return this->LocalToWorld(Vector(this->Position.X, this->Position.Y + (this->Height * this->Scale)));
}

bool BoxCollider::PositionMatch(Vector position)
{
    Vector local_position = this->WorldToLocal(position);

    if (local_position.X < this->Position.X)
        return false;
    if (local_position.Y < this->Position.Y)
        return false;
    if (local_position.X > this->Position.X + (this->Width * this->Scale))
        return false;
    if (local_position.Y > this->Position.Y + (this->Height * this->Scale))
        return false;
    return true;
}

bool BoxCollider::IntersectionMatch(Collider *collider)
{
    if (collider->GetColliderType() == PE::PE_ColliderType_Box)
        return ColliderMath::IntersectionCheckBoxBox(this, dynamic_cast<BoxCollider*>(collider));

    if (collider->GetColliderType() == PE::PE_ColliderType_Bitmap)
        return ColliderMath::IntersectionCheckBoxBitmap(this, dynamic_cast<BitmapCollider*>(collider));

    if (collider->GetColliderType() == PE::PE_ColliderType_Circle)
        return ColliderMath::IntersectionCheckBoxCircle(this, dynamic_cast<CircleCollider*>(collider));

    return this->PositionMatch(collider->Position);
}

#ifdef PE_DEBUG
void BoxCollider::Render(Renderer *r, Camera *c)
{
    if (!Collider::Debug)
        return;

    if (!this->IsOriented())
    {
        PE::Vector root = c->ProjectedPosition(this->Position);
        r->DrawRect(root.X2int(), root.Y2int(), (this->Width * this->Scale), (this->Height * this->Scale), 1, Qt::green);
        return;
    }

    PE::Vector a = c->ProjectedPosition(this->A());
    PE::Vector b = c->ProjectedPosition(this->B());
    PE::Vector cc = c->ProjectedPosition(this->C());
    PE::Vector d = c->ProjectedPosition(this->D());
    r->DrawLine(a.X, a.Y, b.X, b.Y, 1, Qt::green);
    r->DrawLine(b.X, b.Y, cc.X, cc.Y, 1, Qt::green);
    r->DrawLine(cc.X, cc.Y, d.X, d.Y, 1, Qt::green);
    r->DrawLine(d.X, d.Y, a.X, a.Y, 1, Qt::green);
}
#endif
