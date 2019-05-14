//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

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
