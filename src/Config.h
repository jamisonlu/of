/*!
  \file src/Config.h
  \brief Configurations and definitions for Optical Flow algorithms.
  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_CONFIG_H
#define __OF_INTERNAL_CONFIG_H

// STL
#include <cstdlib>

namespace of
{
  // Default threshold for automatic stopping of Horn & Schunck method.
  const double DEFAULT_HS_AUTO_STOP_THRESHOLD = 0.001;

  // Default Lukas & Kanade kernel size.
  const std::size_t DEFAULT_LK_KERNEL_SIZE = 15;

} // end namespace of

#endif // __OF_INTERNAL_CONFIG_H
