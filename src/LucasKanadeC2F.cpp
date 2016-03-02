/*!
  \file src/LucasKanade.cpp

  \brief This class implements the Lucas & Kanade method of estimating optical flow with pyramids.
         Reference: Bouguet, J.-Y. (2000), Pyramidal Implementation of the Lucas Kanade Feature Tracker Description of the algorithm,
                    Intel Corporation Microprocessor Research Labs.
                    http://robots.stanford.edu/cs223b04/algo_tracking.pdf

  \author Douglas Uba
*/

#include "Config.h"
#include "Image.h"
#include "LucasKanade.h"
#include "LucasKanadeC2F.h"
#include "Pyramid.h"

// STL
#include <algorithm>

of::LucasKanadeC2F::LucasKanadeC2F(Image* a, Image* b)
  : OpticalFlow(a, b),
    m_ksize(DEFAULT_LK_KERNEL_SIZE),
    m_maxIterations(1)
{
  m_nLevels = Pyramid::getMaxNumberOfLevels(a);
  m_pyra = new Pyramid(a, m_nLevels);
  m_pyrb = new Pyramid(b, m_nLevels);
}

of::LucasKanadeC2F::LucasKanadeC2F(Image* a, Image* b, std::size_t nLevels)
  : OpticalFlow(a, b),
    m_nLevels(nLevels),
    m_ksize(15),
    m_maxIterations(1)
{
  m_pyra = new Pyramid(a, m_nLevels);
  m_pyrb = new Pyramid(b, m_nLevels);
}

of::LucasKanadeC2F::~LucasKanadeC2F()
{
  delete m_pyra;
  delete m_pyrb;
}

void of::LucasKanadeC2F::compute()
{
  Image* currentU = 0;
  Image* currentV = 0;

  for(int level = m_nLevels; level >= 0; --level)
  {
    Image* a = m_pyra->getLevel(level);
    Image* b = m_pyrb->getLevel(level);

    LucasKanade of(a, b);
    of.setKernelSize(m_ksize);
    of.setMaxNumberOfIterations(m_maxIterations);
    of.compute();

    Image* u = of.getU();
    Image* v = of.getV();

    if(level != m_nLevels)
    {
      // Accumulate flow vectors
      for(std::size_t i = 0; i < u->getNPixels(); ++i)
      {
        u->setPixel(i, u->getPixel(i) + currentU->getPixel(i));
        v->setPixel(i, v->getPixel(i) + currentV->getPixel(i));
      }
    }

    if(level != 0)
    {
      delete currentU;
      delete currentV;

      Size nextsize = m_pyra->getLevel(level - 1)->getSize();

      // Upsampling (u,v)
      currentU = Pyramid::up(u, nextsize);
      currentV = Pyramid::up(v, nextsize);

      // Apply transformation
      Image* warpForward = warp(m_pyra->getLevel(level - 1), currentU, currentV, true);
      Image* warpBackward = warp(m_pyrb->getLevel(level - 1), currentU, currentV, false);

      // Update pyramids for next iteration
      m_pyra->updateLevel(level - 1, warpForward);
      m_pyrb->updateLevel(level - 1, warpBackward);
    }
    else
    {
      m_u = currentU;
      m_v = currentV;
      m_fx = of.getFx()->clone();
      m_fy = of.getFy()->clone();
      m_ft = of.getFt()->clone();
    }
  }
}

void of::LucasKanadeC2F::setKernelSize(std::size_t size)
{
  m_ksize = size;
}

void of::LucasKanadeC2F::setMaxNumberOfIterations(std::size_t n)
{
  m_maxIterations = n;
}

of::Image* of::LucasKanadeC2F::warp(Image* src, Image* u, Image* v, bool isForward) const
{
  const double noDataWarp = std::numeric_limits<double>::max();

  Image* warp = new Image(src->getSize());
  warp->fill(noDataWarp);

  // Apply transformation
  for(int lin = 0; lin < warp->getNLines(); ++lin)
  {
    for(int col = 0; col < warp->getNCols(); ++col)
    {
      double wlin, wcol;
      if(isForward)
      {
        wlin = lin - v->getPixel(lin, col) * 0.5;
        wcol = col - u->getPixel(lin, col) * 0.5;
      }
      else // Backward
      {
        wlin = lin + v->getPixel(lin, col) * 0.5;
        wcol = col + u->getPixel(lin, col) * 0.5;
      }

      // Get pixel part (integer)
      int y = std::max(0, int(std::floor(wlin)));
      int x = std::max(0, int(std::floor(wcol)));

      // Get sub-pixel part (double)
      double alphay = std::abs(wlin - y);
      double alphax = std::abs(wcol - x);

      // Bilinear interpolation
      double a = src->getPixel(y, 0, x, 0);
      double b = src->getPixel(y, 0, x, 1);
      double c = src->getPixel(y, 1, x, 0);
      double d = src->getPixel(y, 1, x, 1);

      double value = (1.0 - alphax) * (1.0 - alphay) * a;
      value += alphax * (1.0 - alphay) * b;
      value += (1.0 - alphax) * alphay * c;
      value += alphax * alphay * d;

      warp->setPixel(lin, col, value);
    }
  }

  return warp;
}
