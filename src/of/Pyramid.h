/*!
  \file src/of/Pyramid.h
  \brief This class represents an image hierarchical pyramid.
  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_PYRAMID_H
#define __OF_INTERNAL_PYRAMID_H

#include "Config.h"

// STL
#include <vector>

namespace of
{
// Forward declarations
  class Image;
  struct Kernel;

  /*!
    \class Pyramid

    \brief This class represents an image hierarchical pyramid.
  */
  class OFEXPORT Pyramid
  {
    public:

      /*!
        \brief Constructor.

        \param image The image that will be used to build the hierarchical pyramid.
        \param nLevels The pyramid number of levels.
        
        \note The Pyramid will not take the ownership of the given image.
      */
      Pyramid(Image* image, std::size_t nLevels);

      /*! \brief Destructor. */
      ~Pyramid();

      /*!
        \brief This method returns all levels of hierarchical pyramid.

        \return All levels of hierarchical pyramid.
      */
      const std::vector<Image*>& getLevels() const;

      /*!
        \brief This method returns the i-th level of the hierarchical pyramid.

        \param i The requested level.

        \return The i-th level of the hierarchical pyramid.
      */
      Image* getLevel(std::size_t i) const;

      /*!
        \brief This method returns the pyramid number of levels.

        \return The pyramid number of levels.
      */
      std::size_t getNLevels() const;

      /*!
        \brief This method updates the i-th level of the hierarchical pyramid.

        \param i The level index.
        \param image The new level value.
      */
      void updateLevel(std::size_t i, Image* image);

      /*!
        \brief This method performs downsampling of the given image.

        \param image The image that will be used.

        \return The downsampling image.
      */
      static Image* down(Image* image);

      /*!
        \brief This method performs upsampling of the given image.

        \param image The image that will be used.

        \return The downsampling image.
      */
      static Image* up(Image* image, const Size& size = Size());

      /*!
        \brief This method computes the maximum number of hierarchical levels based on the given image size.

        \param image The image that will be used.

        \return The maximum number of hierarchical levels based on the given image size.
      */
      static std::size_t getMaxNumberOfLevels(Image* image);

    private:

      /*!
        \brief Internal method that builds the hierarchical pyramid.
      */
      void build();

      /*!
        \brief Static initializer for static members of this class.
      */
      struct Initializer { Initializer(); };

    private:

      std::vector<Image*> m_pyramid;      //!< The hierarchical pyramid.
      static Kernel sm_gkDown;            //!< The gaussian kernel used on downsampling process.
      static Kernel sm_gkUp;              //!< The gaussian kernel used on upsampling process.
      static Initializer sm_initializer;  //!< Static initializer for gaussian kernels.
  };

} // end namespace of

#endif // __OF_INTERNAL_PYRAMID_H
