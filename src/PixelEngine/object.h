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

#include "definitions.h"
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
    class Camera;

    /*!
     * Objects are static transforms that can render to world. Each object has a position relative to world.
     *
     * This is a lowest level component in hierarchy of PixelEngine that all world components are inherited from.
     * Standalone objects are not affected by world physics, if you need that, look at Actor class.
     *
     * Each object can also have multiple childrens and 1 parent
     */
    class Object : public Collectable
    {
        public:
            Object(Object *p = nullptr);
            Object(const Vector &position, Object *p = nullptr);
            ~Object() override;
            virtual void Render(Renderer *r, Camera *c) { (void)r; (void)c; }
            Object *GetParent() { return this->parent; }
            virtual PE_ObjectType GetType();
            bool HasChildren();
            virtual void AddChildren(Object *obj);
            virtual void RemoveChildren(Object *obj);
            //! Set position of this object and all children
            virtual void SetPosition(Vector p);
            virtual void Event_OnCollision(Collider *collider) { (void)collider; }
            virtual void Event_Destroyed() {}
            virtual Vector GetPosition() const;
            void UpdateRecursivelyLastMovement(qint64 time);
            //! Optional optimization function - if returns false, the object will not redraw even if RedrawNeeded is true
            //! it's recommended to implement this for better peformance, especially for games with large world
            virtual bool IsVisibleOnCamera(Camera *c) { (void)c; return true; }
            virtual void SetScale(pe_float_t scale);
            //! Called when object is being destroyed - this will remove all references to other objects and prepare this object
            //! for deletion from operating memory
            //! This will not remove object from any underlying worlds, this function is typically called by world itself.
            //! If you just want to destroy object and not handle its removal from world, then call just Destroy() and let
            //! world perform the clean-up for you
            virtual void DestroyNow();
            //! Schedule object for destruction after specific ms, the destruction is handled by underlying world
            virtual void Destroy(qint64 time = 0);
            bool IsDestroyed() { return this->isDestroyed; };
            //! If positive value, this object will be destroyed after this time value
            qint64 DestroyAfter = -1;
            //! Current absolute position of object in the world - it's not recommended to directly change this, instead call SetPosition()
            Vector Position;
            //! This is relative position to parent object, if it has no parent, then there is no point in using this
            Vector RelativePosition;
            //! Scale of this object, if you want to change scale of whole object including children use ChangeScale
            pe_float_t Scale = 1;
            //! This value is used by physics cache
            qint64 LastMovementUpdate = 0;
            //! If this object explicitly needs to redraw, this is necessary for rendering optimizations
            bool RedrawNeeded = false;

        private:
            bool isDestroyed = false;
            Collectable_SmartPtr<Object> parent;
            QList<Collectable_SmartPtr<Object>> *children = nullptr;
    };
}

#endif // OBJECT_H
