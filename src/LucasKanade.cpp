/*!
  \file src/LucasKanade.cpp

  \brief This class implements the Lucas & Kanade method of estimating optical flow.
         Reference: B. D. Lucas and T. Kanade (1981), An iterative image registration technique with an application to stereo vision.
                    Proceedings of Imaging Understanding Workshop, pages 121--130
                    http://www-cse.ucsd.edu/classes/sp02/cse252/lucaskanade81.pdf

  \author Douglas Uba
*/

#include "Config.h"
#include "Image.h"
#include "LucasKanade.h"

of::LucasKanade::LucasKanade(Image* a, Image* b)
  : OpticalFlow(a, b),
    m_ksize(DEFAULT_LK_KERNEL_SIZE),
    m_maxIterations(1)
{
}

of::LucasKanade::~LucasKanade()
{
}

void of::LucasKanade::compute()
{
  initialize();

  // Auxiliary arrays
  Size size = m_u->getSize();
  Image* sumfx2 = new Image(size);
  Image* sumfy2 = new Image(size);
  Image* sumfxfy = new Image(size);
  Image* sumfxft = new Image(size);
  Image* sumfyft = new Image(size);

  Image* currentImage = m_imga;

  for(std::size_t it = 0; it < m_maxIterations; ++it)
  {
    // Compute derivative images (fx, fy and ft)
    computeDerivativeImages(currentImage, m_imgb);

    // Build equation arrays
    buildMatrix(sumfx2, m_fx, m_fx);
    buildMatrix(sumfy2, m_fy, m_fy);
    buildMatrix(sumfxfy, m_fx, m_fy);
    buildMatrix(sumfxft, m_fx, m_ft);
    buildMatrix(sumfyft, m_fy, m_ft);

    for(std::size_t i = 0; i < size.npixels; ++i)
    {
      double d = sumfx2->getPixel(i) * sumfy2->getPixel(i) - sumfxfy->getPixel(i) * sumfxfy->getPixel(i);

      if(d == 0)
        continue;

      double u = (sumfxfy->getPixel(i) * sumfyft->getPixel(i) - sumfy2->getPixel(i) * sumfxft->getPixel(i)) / d;
      double v = (sumfxft->getPixel(i) * sumfxfy->getPixel(i) - sumfx2->getPixel(i) * sumfyft->getPixel(i)) / d;

      m_u->setPixel(i, m_u->getPixel(i) + u);
      m_v->setPixel(i, m_v->getPixel(i) + v);
    }

    if(m_maxIterations == 1)
      break;

    if(currentImage != m_imga)
      delete currentImage;

    // Iterative warp
    currentImage = warp(m_imga, m_u, m_v);
  }

  delete sumfx2;
  delete sumfy2;
  delete sumfxfy;
  delete sumfxft;
  delete sumfyft;
}

void of::LucasKanade::setKernelSize(std::size_t size)
{
  m_ksize = size;
}

void of::LucasKanade::setMaxNumberOfIterations(std::size_t n)
{
  m_maxIterations = n;
}

void of::LucasKanade::buildMatrix(Image* dst, Image* a, Image* b) const
{
  for(int lin = 0; lin < dst->getNLines(); ++lin)
  {
    for(int col = 0; col < dst->getNCols(); ++col)
    {
      double v = 0.0;

      // for each window/kernel pixel
      for(int lw = -(int(m_ksize) / 2); lw <= int(m_ksize) / 2; ++lw)
        for(int cw = -(int(m_ksize) / 2); cw <= int(m_ksize) / 2; ++cw)
          //v += a->getPixel(lin, lw, col, cw) * b->getPixel(lin, lw, col, cw);
          v += a->getPixel(lin + lw, col + cw) * b->getPixel(lin + lw, col + cw);

      dst->setPixel(lin, col, v);
    }
  }
}
