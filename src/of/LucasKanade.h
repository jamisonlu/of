/*!
  \file src/of/LucasKanade.h

  \brief This class implements the Lucas & Kanade method of estimating optical flow.
         Reference: B. D. Lucas and T. Kanade (1981), An iterative image registration technique with an application to stereo vision.
                    Proceedings of Imaging Understanding Workshop, pages 121--130
                    http://www-cse.ucsd.edu/classes/sp02/cse252/lucaskanade81.pdf

  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_LUCAS_KANADE_H
#define __OF_INTERNAL_LUCAS_KANADE_H

#include "OpticalFlow.h"

namespace of
{
  /*!
    \class LucasKanade

    \brief This class implements the Lucas & Kanade method of estimating optical flow.
  */
  class OFEXPORT LucasKanade : public OpticalFlow
  {
    public:

      /*!
        \brief Constructor.

        \param a The first image.
        \param b The second image.

        \note The LucasKanade will not take the ownership of the given images.
      */
      LucasKanade(Image* a, Image* b);

      /*! \brief Destructor. */
      ~LucasKanade();

      void compute();

      /*!
        \brief This methods sets the kernel size that will be used.

        \param size The kernel size. e.g. (5 = 5 x 5)
      */
      void setKernelSize(std::size_t size);

      /*!
        \brief This methods sets the maximum number of iterations

        \param n The maximum number of iterations.
      */
      void setMaxNumberOfIterations(std::size_t n);

    private:

      void buildMatrix(Image* dst, Image* a, Image* b) const;

    private:

      std::size_t m_ksize;         //!< Kernel size. (Default: 15 x 15)
      std::size_t m_maxIterations; //!< Maximum number of iterations. (Default: 1)
  };

} // end namespace of

#endif // __OF_INTERNAL_LUCAS_KANADE_H
