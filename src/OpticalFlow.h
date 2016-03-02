/*!
  \file src/OpticalFlow.h
  \brief Base class for optical flow algorithms.
  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_OPTICAL_FLOW_H
#define __OF_INTERNAL_OPTICAL_FLOW_H

// STL
#include <string>

namespace of
{
// Forward declarations
  class Image;

  /*!
    \class OpticalFlow

    \brief Base class for optical flow algorithms.
  */
  class OpticalFlow
  {
    public:

      /*!
        \brief Constructor.

        \param a The first image.
        \param b The second image.

        \note The OpticalFlow will not take the ownership of the given images.
      */
      OpticalFlow(Image* a, Image* b);

      /*! \brief Virtual destructor. */
      virtual ~OpticalFlow();

      /*!
        \brief Pure virtual method that computes the optical flow between input images.
      */
      virtual void compute() = 0;

      /*!
        \brief This method returns the u (i.e. x) coordinates of flow vectors found.

        \return The u (i.e. x) coordinates of flow vectors found.
      */
      Image* getU() const;

      /*!
        \brief This method returns the v (i.e. y) coordinates of flow vectors found.

        \return The v (i.e. y) coordinates of flow vectors found.
      */
      Image* getV() const;

      /*!
        \brief This method returns the derivative image (Fx) computed.

        \return The derivative image (Fx) computed.
      */
      Image* getFx() const;

      /*!
        \brief This method returns the derivative image (Fy) computed.

        \return The derivative image (Fy) computed.
      */
      Image* getFy() const;

      /*!
        \brief This method returns the derivative image (Ft) computed.

        \return The derivative image (Ft) computed.
      */
      Image* getFt() const;

      /*!
        \brief This method returns the warped image.

        \return The warped image.
      */
      Image* getWarped();

      /*!
        \brief This method returns the error image. i.e. (warped - b)

        \return The error image.
      */
      Image* getError();

      /*!
        \brief This method saves the (u,v) coordinates found to Optical Flow Middlebury (.flo) file.

        \note Reference: http://vision.middlebury.edu/flow/code/flow-code/README.txt
      */
      void save(const std::string& path) const;

    protected:

      /*!
        \brief Internal method that initializes this class.
      */
      void initialize();

      /*!
        \brief Internal method that computes the derivative images
               necessary to optical flow algorithms.
      */
      void computeDerivativeImages();

      /*!
        \brief Internal method that computes the derivative images
               necessary to optical flow algorithms.

        \param a The first image.
        \param b The second image.
      */
      void computeDerivativeImages(Image* a, Image* b);

      /*!
        \brief Internal method that applies a warping transformation which maps all positions
               in one image plane to positions in a second plane based on the given vectors.

        \param src The image that will be transformed.
        \param u The u coordinates.
        \param v The v coordinates.

        \note The transformation is 'forward'.

        \return The transformed image.
      */
      Image* warp(Image* src, Image* u, Image* v) const;

    protected:

      Image* m_imga;   //!< The first image.
      Image* m_imgb;   //!< The second image.
      Image* m_fx;     //!< The x derivative image.
      Image* m_fy;     //!< The y derivative image.
      Image* m_ft;     //!< The t derivative image.
      Image* m_u;      //!< The u coordinates image.
      Image* m_v;      //!< The v coordinates image.
      Image* m_warped; //!< The warped image.
      Image* m_error;  //!< The error image.
  };
} // end namespace of

#endif // __OF_INTERNAL_OPTICAL_FLOW_H
