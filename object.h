//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef OBJECT_H
#define OBJECT_H

#include "vector.h"
#include "GC/collectable.h"
#include "GC/collectable_smartptr.h"
#include <QList>

namespace PE
{
    class Renderer;

    enum PE_ObjectType
    {
        PE_ObjectType_Object,
        PE_ObjectType_Actor,
        PE_ObjectType_Pawn,
        PE_ObjectType_Collider,
        PE_ObjectType_Terrain
    };

    class Collider;

    //! Objects are static transforms that can render to world. Basically every world component is inherited from this class.
    //! Each object can also have multiple childrens and 1 parent
    class Object : public Collectable
    {
        public:
            Object(Object *p = nullptr);
            ~Object() override;
            virtual void Render(Renderer *r) { (void)r; };
            Object *GetParent() { return this->parent; };
            virtual PE_ObjectType GetType();
            bool HasChildren();
            virtual void AddChildren(Object *obj);
            //! Set position of this object and all children
            virtual void SetPosition(Vector p);
            virtual void Event_OnCollision(Collider *collider) { (void)collider; }
            virtual Vector GetPosition() const;
            void UpdateRecursivelyLastMovement(qint64 time);
            //! Current absolute position of object in the world
            Vector Position;
            //! This is relative position to parent object, if it has no parent, then there is no point in using this
            Vector RelativePosition;
            //! This value is used by physics cache
            qint64 LastMovementUpdate = 0;

        private:
            Collectable_SmartPtr<Object> parent;
            QList<Collectable_SmartPtr<Object>> *children = nullptr;
    };
}

#endif // OBJECT_H
