/*!
  \file src/LucasKanadeC2F.h

  \brief This class implements the Lucas & Kanade method of estimating optical flow with pyramids.
         Reference: Bouguet, J.-Y. (2000), Pyramidal Implementation of the Lucas Kanade Feature Tracker Description of the algorithm,
                    Intel Corporation Microprocessor Research Labs.
                    http://robots.stanford.edu/cs223b04/algo_tracking.pdf

  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_LUCAS_KANADE_C2F_H
#define __OF_INTERNAL_LUCAS_KANADE_C2F_H

#include "OpticalFlow.h"

namespace of
{
// Forward declaration
  class Pyramid;

  /*!
    \class LucasKanadeC2F

    \brief This class implements the Lucas & Kanade method of estimating optical flow with pyramids.
  */
  class LucasKanadeC2F : public OpticalFlow
  {
    public:

      /*!
        \brief Constructor.

        \param a The first image.
        \param b The second image.

        \note The number of levels used will be compute automatically. i.e. maximum number of levels.

        \note The LucasKanadeC2F will not take the ownership of the given images.
      */
      LucasKanadeC2F(Image* a, Image* b);

      /*!
        \brief Constructor.

        \param a The first image.
        \param b The second image.
        \param nLevels Number of levels that will be used.

        \note The LucasKanadeC2F will not take the ownership of the given images.
      */
      LucasKanadeC2F(Image* a, Image* b, std::size_t nLevels);

      /*! \brief Destructor. */
      ~LucasKanadeC2F();

      void compute();

      /*!
        \brief This methods sets the kernel size that will be used.

        \param size The kernel size. e.g. (5 = 5 x 5)
      */
      void setKernelSize(std::size_t size);

      /*!
        \brief This methods sets the maximum number of iterations for each level.

        \param n The maximum number of iterations for each level.
      */
      void setMaxNumberOfIterations(std::size_t n);

    private:

      /*!
        \brief Internal method that applies a warping transformation which maps all positions
               in one image plane to positions in a second plane based on the given vectors.

        \param src The image that will be transformed.
        \param u The u coordinates.
        \param v The v coordinates.
        \param isForward A flag that indicates if the transformation is 'forward'. Case false, it is 'backward'.

        \return The transformed image.
      */
      Image* warp(Image* src, Image* u, Image* v, bool isForward) const;

    private:

      std::size_t m_nLevels;       //!< Number of levels that will be used.
      Pyramid* m_pyra;             //!< Internal pyramid for image A.
      Pyramid* m_pyrb;             //!< Internal pyramid for image B.
      std::size_t m_ksize;         //!< Kernel size. (Default: 5 x 5)
      std::size_t m_maxIterations; //!< Maximum number of iterations for each level.
  };

} // end namespace of

#endif // __OF_INTERNAL_LUCAS_KANADE_C2F_H
