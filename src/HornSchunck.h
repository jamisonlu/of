/*!
  \file src/HornSchunck.h

  \brief This class implements the Horn & Schunck method for estimating optical flow.
         Reference: B.K.P. Horn and B.G. Schunck, "Determining optical flow." Artificial Intelligence, vol 17, pp 185–203, 1981
                    http://dspace.mit.edu/handle/1721.1/6337

  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_HORN_SCHUNCK_H
#define __OF_INTERNAL_HORN_SCHUNCK_H

#include "OpticalFlow.h"

namespace of
{
  /*!
    \class HornSchunck

    \brief This class implements the Horn & Schunck method of estimating optical flow.
  */
  class HornSchunck : public OpticalFlow
  {
    public:

      /*!
        \brief Constructor.

        \param a The first image.
        \param b The second image.

        \note The HornSchunck will not take the ownership of the given images.
      */
      HornSchunck(Image* a, Image* b);

      /*! \brief Destructor. */
      ~HornSchunck();

      void compute();

      /*!
        \brief This methods sets the Horn-Schunck method alpha parameter that will be used.

        \param alpha The Horn-Schunck method alpha parameter that will be used.
      */
      void setAlpha(double alpha);

      /*!
        \brief This methods sets the maximum number of iterations

        \param n The maximum number of iterations.
      */
      void setMaxNumberOfIterations(std::size_t n);

      /*!
        \brief This methods sets the threshold for automatic stopping.

        \param e The threshold for automatic stopping.
      */
      void setAutoStopThreshold(double e);

    private:

      void computeLocalAvg(double* avg, Image* coords);

    private:

      double m_alpha;              //!< Horn-Schunck alpha parameter.
      std::size_t m_maxIterations; //!< Maximum number of iterations.
      double m_e;                  //!< Automatic stop threshold.

  };

} // end namespace of

#endif // __OF_INTERNAL_HORN_SCHUNCK_H
