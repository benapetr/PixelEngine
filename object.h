// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

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
    class Camera;

    //! Objects are static transforms that can render to world. Basically every world component is inherited from this class.
    //! Each object can also have multiple childrens and 1 parent
    class Object : public Collectable
    {
        public:
            Object(Object *p = nullptr);
            ~Object() override;
            virtual void Render(Renderer *r, Camera *c) { (void)r; (void)c; }
            Object *GetParent() { return this->parent; }
            virtual PE_ObjectType GetType();
            bool HasChildren();
            virtual void AddChildren(Object *obj);
            //! Set position of this object and all children
            virtual void SetPosition(Vector p);
            virtual void Event_OnCollision(Collider *collider) { (void)collider; }
            virtual Vector GetPosition() const;
            void UpdateRecursivelyLastMovement(qint64 time);
            //! Optional optimization function - if returns false, the object will not redraw even if RedrawNeeded is true
            virtual bool IsVisibleOnCamera(Camera *c) { (void)c; return true; }
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
            //! Current absolute position of object in the world
            Vector Position;
            //! This is relative position to parent object, if it has no parent, then there is no point in using this
            Vector RelativePosition;
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
