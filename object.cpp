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
#include <QDateTime>

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

void Object::RemoveChildren(Object *obj)
{
    if (this->children == nullptr)
        return;
    this->children->removeAll(obj);
}

void Object::SetPosition(Vector p)
{
    this->Position = p;
    if (this->children == nullptr)
        return;
    // This is a hack due to unclear Qt bug
    // if iteration is not done inside of a list copy, it randomly crashes
    QList<Collectable_SmartPtr<Object>> cl(*this->children);
    foreach (Object *c, cl)
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

void Object::DestroyNow()
{
    this->Event_Destroyed();
    this->isDestroyed = true;
    // Clear references to children objects
    if (this->children != nullptr)
    {
        foreach (Object *o, *this->children)
            o->DestroyNow();
        this->children->clear();
    }
    // Clear reference to parent
    this->parent = nullptr;
}

void Object::Destroy(qint64 time)
{
    this->DestroyAfter = QDateTime::currentDateTime().toMSecsSinceEpoch() + time;
}
