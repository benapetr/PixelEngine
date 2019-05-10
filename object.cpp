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
    delete this->childrens;
}

Object *Object::GetParent()
{
    return this->parent;
}

PE_ObjectType Object::GetType()
{
    return PE_ObjectType_Object;
}

bool Object::HasChildren()
{
    return !(this->childrens == nullptr || this->childrens->isEmpty());
}
