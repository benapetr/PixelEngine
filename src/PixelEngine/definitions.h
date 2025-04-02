//! \file definitions.h
//! \brief Header file containing global definitions, constants, and macros for the Pixel Engine.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

//! \def PE_VERSION_STRING
//! Defines the version string of the Pixel Engine.
#define PE_VERSION_STRING "1.0.0.0"

//! \def PE_MAX_CM_SIZE
//! Maximum size of the collision matrix.
#define PE_MAX_CM_SIZE 64

//! \def PE_RING_LOG_SIZE
//! Size of the ring log buffer.
#define PE_RING_LOG_SIZE  600

//! \def PE_PI
//! Mathematical constant Pi.
#define PE_PI 3.14159265

//! \def PE_PI_RAD_CNV
//! Conversion factor from degrees to radians (Pi / 180).
#define PE_PI_RAD_CNV 1.74532925

//! \def PE_RAD_DEG_CNV
//! Conversion factor from radians to degrees.
#define PE_RAD_DEG_CNV 57.2958

//! \def PE_BITSET_MAX
//! Maximum size of a bitmap.
#define PE_BITSET_MAX 10000

#ifdef DOUBLE_FLOAT
//! \typedef pe_float_t
//! Floating-point type used in the engine (double if DOUBLE_FLOAT is defined).
typedef double pe_float_t;
#else
//! \typedef pe_float_t
//! Floating-point type used in the engine (float by default).
typedef float pe_float_t;
#endif

// Overview of define switches that can be defined (but are not enabled by default)
// Enable complete debugging (big performance hit)
//#define PE_DEBUG

// Enable performance metrics
//#define PE_METRICS

// stolen from boost/current_function.hpp
// enhanced by Petr Bena, keep the enhancement in case you wanted to update this

#ifndef __LINE__
    #define __LINE__ 0
#endif

#ifndef __FILE__
    #define __FILE__ "{unknown}"
#endif

//! \def PE_FILE_LINE
//! Macro to generate a string containing the file and line number.
#define PE_FILE_LINE QString("@") + QString(__FILE__) + QString(":") + QString::number(__LINE__)

//! \def BOOST_CURRENT_FUNCTION
//! Macro to generate a string containing the current function name and file/line information.
#ifndef BOOST_CURRENT_FUNCTION
    #if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600))
        #define BOOST_CURRENT_FUNCTION QString(__PRETTY_FUNCTION__) + PE_FILE_LINE
    #elif defined(__DMC__) && (__DMC__ >= 0x810)
        #define BOOST_CURRENT_FUNCTION QString(__PRETTY_FUNCTION__) + PE_FILE_LINE
    #elif defined(__FUNCSIG__)
        #define BOOST_CURRENT_FUNCTION QString(__FUNCSIG__) + PE_FILE_LINE
    #elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
        #define BOOST_CURRENT_FUNCTION QString(__FUNCTION__) + PE_FILE_LINE
    #elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
        #define BOOST_CURRENT_FUNCTION QString(__FUNC__) + PE_FILE_LINE
    #elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
        #define BOOST_CURRENT_FUNCTION QString(__func__) + PE_FILE_LINE
    #else
        #define BOOST_CURRENT_FUNCTION QString("(unknown)") + PE_FILE_LINE
    #endif
#endif

#endif // DEFINITIONS_H
