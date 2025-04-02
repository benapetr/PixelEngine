//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019 - 2025

//! \file engine.h
//! \brief Header file for the Engine class, the core of Pixel Engine.

#ifndef ENGINE_H
#define ENGINE_H

#include "definitions.h"
#include <QDateTime>

//! \def PE_WRITE_RING(text)
//! Macro to write text to the ring log of the engine.
#define PE_WRITE_RING(text) { PE::Engine::GetEngine()->RL->WriteText(text); }

namespace PE
{
    class RingLog;
    class GC;

    //! \class Engine
    //! \brief The core class of Pixel Engine, responsible for initialization and managing subsystems.
    class Engine
    {
        public:
            //! \brief Initialize the engine.
            //! \param multithreaded_gc If true, enables multithreaded garbage collection.
            static void Initialize(bool multithreaded_gc = true);

            //! \brief Get the singleton instance of the engine.
            //! \return Pointer to the engine instance.
            static Engine *GetEngine();

            //! \brief Get the version of the engine.
            //! \return A QString containing the version.
            static QString GetVersion();

            //! \brief Constructor for the Engine class.
            //! \param multithreaded_gc If true, enables multithreaded garbage collection.
            Engine(bool multithreaded_gc = true);

            //! \brief Destructor for the Engine class.
            virtual ~Engine();

            //! \brief Pointer to the ring log for logging messages.
            RingLog *RL = nullptr;

            //! \brief Get the garbage collector instance.
            //! \return Pointer to the garbage collector.
            GC *GetGC() const;

        private:
            static Engine *engine; //!< Singleton instance of the engine.
            GC *gc; //!< Pointer to the garbage collector.
            QDateTime startupTime; //!< Timestamp of when the engine was started.
    };
}

#endif // ENGINE_H
