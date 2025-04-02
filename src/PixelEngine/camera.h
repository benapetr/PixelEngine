//! \file camera.h
//! \brief Header file for the Camera class, representing a view into the game world.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

namespace PE
{
    //! \class Camera
    //! \brief Represents a camera view in the game world.
    //! 
    //! The Camera class provides functionality to project world positions relative to the camera's point of view.
    class Camera
    {
        public:
            //! \brief Default constructor for the Camera class.
            Camera();

            //! \brief Get the negated position of the camera.
            //! \return A Vector representing the negated position.
            Vector GetNegated();

            //! \brief Project a world position relative to the camera's point of view.
            //! \param position The world position to project.
            //! \return A Vector representing the projected position.
            Vector ProjectedPosition(Vector position);

            //! \brief The position of the camera in the world.
            Vector Position;
    };
}

#endif // CAMERA_H
