/*!
  \file src/OpticalFlow.cpp
  \brief Base class for optical flow algorithms.
  \author Douglas Uba
*/

#include "Exception.h"
#include "Image.h"
#include "OpticalFlow.h"

// STL
#include <algorithm>
#include <cstdint>
#include <fstream>

of::OpticalFlow::OpticalFlow(Image* a, Image* b)
  : m_imga(a),
    m_imgb(b),
    m_fx(0),
    m_fy(0),
    m_ft(0),
    m_u(0),
    m_v(0),
    m_warped(0),
    m_error(0)
{
  if(m_imga->getSize() != m_imgb->getSize())
    throw Exception("The images must be the same size");
}

of::OpticalFlow::~OpticalFlow()
{
  delete m_fx;
  delete m_fy;
  delete m_ft;
  delete m_u;
  delete m_v;
  delete m_warped;
  delete m_error;
}

of::Image* of::OpticalFlow::getU() const
{
  return m_u;
}

of::Image* of::OpticalFlow::getV() const
{
  return m_v;
}

of::Image* of::OpticalFlow::getFx() const
{
  return m_fx;
}

of::Image* of::OpticalFlow::getFy() const
{
  return m_fy;
}

of::Image* of::OpticalFlow::getFt() const
{
  return m_ft;
}

of::Image* of::OpticalFlow::getWarped()
{
  if(m_warped)
    return m_warped;

  m_warped = warp(m_imga, m_u, m_v);

  return m_warped;
}

of::Image* of::OpticalFlow::getError()
{
  if(m_error)
    return m_error;

  Image* warped = getWarped();

  m_error = new Image(warped->getSize());

  for(std::size_t i = 0; i < warped->getSize().npixels; ++i)
    m_error->setPixel(i, std::abs(m_imgb->getPixel(i) - warped->getPixel(i)));

  return m_error;
}

void of::OpticalFlow::save(const std::string& path) const
{
  std::ofstream file(path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);

  // Write 'magic number'
  const std::string check = "PIEH";
  file.write(check.c_str(), check.size());

  // Get (u,v) dimension
  std::size_t nlines = m_u->getNLines();
  std::size_t ncols  = m_u->getNCols();

  // Write number of columns
  file.write((const char*)&ncols, sizeof(int32_t));

  // Write number of lines
  file.write((const char*)&nlines, sizeof(int32_t));

  double* ubuffer = m_u->getBuffer();
  double* vbuffer = m_v->getBuffer();

  // Write values for u and v, interleaved, in row order
  for(std::size_t i = 0; i < nlines * ncols; ++i)
  {
    float u = float(ubuffer[i]); float v = float(vbuffer[i]);
    file.write((const char*)&u, sizeof(float));
    file.write((const char*)&v, sizeof(float));
  }

  file.close();
}

void of::OpticalFlow::initialize()
{
  Size size = m_imga->getSize();

  // Initialize derivative images
  m_fx = new Image(size);
  m_fy = new Image(size);
  m_ft = new Image(size);

  // Initialize flow vectors
  m_u = new Image(size);
  m_v = new Image(size);

  // Fill internal images
  for(std::size_t i = 0; i < size.npixels; ++i)
  {
    m_fx->setPixel(i, 0.0);
    m_fy->setPixel(i, 0.0);
    m_ft->setPixel(i, 0.0);
    m_u->setPixel(i, 0.0);
    m_v->setPixel(i, 0.0);
  }
}

void of::OpticalFlow::computeDerivativeImages()
{
  computeDerivativeImages(m_imga, m_imgb);
}

void of::OpticalFlow::computeDerivativeImages(Image* a, Image* b)
{
  for(std::size_t lin = 0; lin < m_fx->getNLines(); ++lin)
  {
    for(std::size_t col = 0; col < m_fx->getNCols(); ++col)
    {
      // fx
      m_fx->setPixel(lin, col, (1.0 / 4.0) *
        (a->getPixel(lin, col + 1) - a->getPixel(lin, col)
        + a->getPixel(lin + 1, col + 1) - a->getPixel(lin + 1, col)
        + b->getPixel(lin, col + 1) - b->getPixel(lin, col)
        + b->getPixel(lin + 1, col + 1) - b->getPixel(lin + 1, col)));
      // fy
      m_fy->setPixel(lin, col, (1.0 / 4.0) *
        (a->getPixel(lin + 1, col) - a->getPixel(lin, col)
        + a->getPixel(lin + 1, col + 1) - a->getPixel(lin, col + 1)
        + b->getPixel(lin + 1, col) - b->getPixel(lin, col)
        + b->getPixel(lin + 1, col + 1) - b->getPixel(lin, col + 1)));
      // ft
      m_ft->setPixel(lin, col, (1.0 / 4.0) *
        (b->getPixel(lin, col) - a->getPixel(lin, col)
        + b->getPixel(lin, col + 1) - a->getPixel(lin, col + 1)
        + b->getPixel(lin + 1, col) - a->getPixel(lin + 1, col)
        + b->getPixel(lin + 1, col + 1) - a->getPixel(lin + 1, col + 1)));
    }
  }
}

of::Image* of::OpticalFlow::warp(Image* src, Image* u, Image* v) const
{
  Image* warp = new Image(src->getSize(), src->getNoDataValue());

  // Apply transformation
  for(int lin = 0; lin < warp->getNLines(); ++lin)
  {
    for(int col = 0; col < warp->getNCols(); ++col)
    {
      // Compute destination pixel
      double wlin = lin - v->getPixel(lin, col);
      double wcol = col - u->getPixel(lin, col);

      // Get pixel part
      int y = std::max(0, int(std::floor(wlin)));
      int x = std::max(0, int(std::floor(wcol)));

      // Get sub-pixel part
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
