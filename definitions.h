// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PE_VERSION_STRING "1.0.0.0"
#define PE_RING_LOG_SIZE  600

// Math
#define PE_PI 3.14159265

// PI / 180 for radian
#define PE_PI_RAD_CNV 1.74532925

// Radians to degree
#define PE_RAD_DEG_CNV 57.2958

// Bitmap size
#define PE_BITSET_MAX 10000

// stolen from boost/current_function.hpp
// enhanced by Petr Bena, keep the enhancement in case you wanted to update this

#ifndef __LINE__
    #define __LINE__ 0
#endif

#ifndef __FILE__
    #define __FILE__ "{unknown}"
#endif

#define PE_FILE_LINE QString("@") + QString(__FILE__) + QString(":") + QString::number(__LINE__)

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
