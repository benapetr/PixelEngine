//! \file object.h
//! \brief Header file for the Object class, the base class for all world components in Pixel Engine.

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

    //! \enum PE_ObjectType
    //! \brief Enumerates the types of objects in the Pixel Engine.
    enum PE_ObjectType
    {
        PE_ObjectType_Object, //!< Generic object type.
        PE_ObjectType_Actor, //!< Actor type, affected by physics.
        PE_ObjectType_Pawn, //!< Pawn type, controllable by external factors.
        PE_ObjectType_Collider, //!< Collider type, used for physics collisions.
        PE_ObjectType_Terrain //!< Terrain type, representing the game world surface.
    };

    class Collider;
    class Camera;

    //! \class Object
    //! \brief Base class for all world components in the Pixel Engine.
    //! 
    //! Objects are static transforms that can render to the world. Each object has a position relative to the world.
    //! Standalone objects are not affected by world physics. For physics-enabled objects, see the Actor class.
    //! Each object can have multiple children and one parent.
    class Object : public Collectable
    {
        public:
            //! \brief Constructor for the Object class.
            //! \param p The parent object, if any.
            Object(Object *p = nullptr);

            //! \brief Constructor for the Object class with a specified position.
            //! \param position The position of the object.
            //! \param p The parent object, if any.
            Object(const Vector &position, Object *p = nullptr);

            //! \brief Destructor for the Object class.
            ~Object() override;

            //! \brief Render the object using the specified renderer and camera.
            //! \param r The renderer to use.
            //! \param c The camera to use.
            virtual void Render(Renderer *r, Camera *c) { (void)r; (void)c; }

            //! \brief Get the parent of this object.
            //! \return A pointer to the parent object.
            Object *GetParent() { return this->parent; }

            //! \brief Get the type of this object.
            //! \return The type of the object as a PE_ObjectType.
            virtual PE_ObjectType GetType();

            //! \brief Check if the object has children.
            //! \return True if the object has children, false otherwise.
            bool HasChildren();

            //! \brief Add a child object.
            //! \param obj The child object to add.
            virtual void AddChildren(Object *obj);

            //! \brief Remove a child object.
            //! \param obj The child object to remove.
            virtual void RemoveChildren(Object *obj);

            //! \brief Set the position of this object and all its children.
            //! \param p The new position.
            virtual void SetPosition(Vector p);

            //! \brief Handle a collision event.
            //! \param collider The collider involved in the collision.
            virtual void Event_OnCollision(Collider *collider) { (void)collider; }

            //! \brief Handle the destruction event of the object.
            virtual void Event_Destroyed() {}

            //! \brief Get the current position of the object.
            //! \return The position as a Vector.
            virtual Vector GetPosition() const;

            //! \brief Update the last movement timestamp recursively for this object and its children.
            //! \param time The timestamp to set.
            void UpdateRecursivelyLastMovement(qint64 time);

            //! Optional optimization function - if returns false, the object will not redraw even if RedrawNeeded is true
            //! it's recommended to implement this for better peformance, especially for games with large world
            virtual bool IsVisibleOnCamera(Camera *c) { (void)c; return true; }

            //! \brief Set the scale of the object.
            //! \param scale The new scale value.
            virtual void SetScale(pe_float_t scale);
            
            //! Called when object is being destroyed - this will remove all references to other objects and prepare this object
            //! for deletion from operating memory
            //! This will not remove object from any underlying worlds, this function is typically called by world itself.
            //! If you just want to destroy object and not handle its removal from world, then call just Destroy() and let
            //! world perform the clean-up for you
            virtual void DestroyNow();

            //! \brief Schedule the object for destruction after a specified time.
            //! \param time The time in milliseconds after which the object will be destroyed.
            virtual void Destroy(qint64 time = 0);

            //! \brief Check if the object is destroyed.
            //! \return True if the object is destroyed, false otherwise.
            bool IsDestroyed() { return this->isDestroyed; };

            qint64 DestroyAfter = -1; //!< Time after which the object will be destroyed, if positive.
            Vector Position; //!< Current absolute position of the object in the world.
            Vector RelativePosition; //!< Relative position to the parent object.
            pe_float_t Scale = 1; //!< Scale of the object.
            qint64 LastMovementUpdate = 0; //!< Timestamp of the last movement update.
            bool RedrawNeeded = false; //!< Indicates if the object needs to be redrawn.

        private:
            bool isDestroyed = false; //!< Indicates if the object is destroyed.
            Collectable_SmartPtr<Object> parent; //!< Pointer to the parent object.
            QList<Collectable_SmartPtr<Object>> *children = nullptr; //!< List of child objects.
    };
}

#endif // OBJECT_H
