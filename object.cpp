// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "object.h"

using namespace PE;

Object::Object(Object *p)
{
    this->parent = p;
}

Object::~Object()
{
    delete this->children;
}

PE_ObjectType Object::GetType()
{
    return PE_ObjectType_Object;
}

bool Object::HasChildren()
{
    return !(this->children == nullptr || this->children->isEmpty());
}

void Object::AddChildren(Object *obj)
{
    if (this->children == nullptr)
        this->children = new QList<Collectable_SmartPtr<Object>>();
    this->children->append(obj);
    obj->parent = this;
}

void Object::SetPosition(Vector p)
{
    this->Position = p;
    if (this->children == nullptr)
        return;
    foreach (Object *c, *this->children)
    {
        c->SetPosition(p + c->RelativePosition);
    }
}

Vector Object::GetPosition() const
{
    return this->Position;
}

void Object::UpdateRecursivelyLastMovement(qint64 time)
{
    this->LastMovementUpdate = time;
    if (!this->HasChildren())
        return;

    foreach (Object *c, *this->children)
    {
        c->UpdateRecursivelyLastMovement(time);
    }
}

void Object::Destroy()
{
    this->isDestroyed = true;
    // Clear references to children objects
    if (this->children != nullptr)
    {
        foreach (Object *o, *this->children)
            o->Destroy();
        this->children->clear();
    }
    // Clear reference to parent
    this->parent = nullptr;
}
