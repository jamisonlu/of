/*!
  \file src/of/Config.h
  \brief Configurations and definitions for Optical Flow algorithms.
  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_CONFIG_H
#define __OF_INTERNAL_CONFIG_H

// STL
#include <cstdlib>

/*!
  \def DEFAULT_HS_AUTO_STOP_THRESHOLD

  \brief Default threshold for automatic stopping of Horn & Schunck method.
*/
#define OF_DEFAULT_HS_AUTO_STOP_THRESHOLD 0.001

/*!
  \def DEFAULT_LK_KERNEL_SIZE

  \brief Default Lukas & Kanade kernel size.
*/
#define OF_DEFAULT_LK_KERNEL_SIZE 15

/** @name DLL/LIB Module
*  Flags for building Optical Flow as a DLL or as a Static Library
*/
//@{

/*!
  \def OFEXPORT

  \brief Use this macro in order to export/import classes and functions from this module.

  \note If you want to compile Optical Flow as DLL in Windows, remember to insert OFDLL into the project's list of defines.

  \note If you want to compile Optical Flow as an Static Library under Windows, remember to insert the OFSTATIC flag into the project list of defines.
*/
#ifdef WIN32

  #ifdef _MSC_VER
    #pragma warning( disable : 4251 )
    #pragma warning( disable : 4275 )
    #pragma warning( disable : 4290 )
  #endif

  #ifdef OFSTATIC
    #define OFEXPORT                          // Don't need to export/import. It is a static library
  #elif OFDLL
    #define OFEXPORT  __declspec(dllexport)   // export DLL information
  #else
    #define OFEXPORT  __declspec(dllimport)   // import DLL information
  #endif
#else
  #define OFEXPORT
#endif

//@}

#endif // __OF_INTERNAL_CONFIG_H
