/*!
  \file src/Image.cpp
  \brief This class represents a single-band digital image.
  \author Douglas Uba
*/

// Optical Flow
#include "Image.h"

of::Image::Image(const Size& size, double noDataValue)
  : m_size(size),
    m_noDataValue(noDataValue)
{
  m_buffer = new double[m_size.npixels];
}

of::Image::Image(std::size_t nlines, std::size_t ncols, double noDataValue)
  : m_size(nlines, ncols),
    m_noDataValue(noDataValue)
{
  m_buffer = new double[m_size.npixels];
}

of::Image::Image(double* buffer, std::size_t nlines, std::size_t ncols, double noDataValue)
  : m_buffer(buffer),
    m_size(nlines, ncols),
    m_noDataValue(noDataValue)
{
}

of::Image::Image(const Image& rhs)
  : m_size(rhs.m_size),
    m_noDataValue(rhs.m_noDataValue)
{
  m_buffer = new double[m_size.npixels];

  for(std::size_t i = 0; i < m_size.npixels; ++i)
    m_buffer[i] = rhs.m_buffer[i];
}

of::Image::~Image()
{
  delete [] m_buffer;
}

double* of::Image::getBuffer() const
{
  return m_buffer;
}

const of::Size& of::Image::getSize() const
{
  return m_size;
}

std::size_t of::Image::getNLines() const
{
  return m_size.nlines;
}

std::size_t of::Image::getNCols() const
{
  return m_size.ncols;
}

std::size_t of::Image::getNPixels() const
{
  return m_size.npixels;
}

double of::Image::getNoDataValue() const
{
  return m_noDataValue;
}

bool of::Image::isNoData(int lin, int col) const
{
  return getPixel(lin, col) == m_noDataValue;
}

double of::Image::getPixel(int lin, int col) const
{
  m_size.clamp(lin, col);

  return m_buffer[index(lin, col)];
}

double of::Image::getPixel(int lin, int dl, int col, int dc) const
{
  lin = reflect(lin, dl, m_size.nlines);
  col = reflect(col, dc, m_size.ncols);

  return getPixel(lin, col);
}

double of::Image::getPixel(std::size_t i) const
{
  return m_buffer[i];
}

void of::Image::setPixel(int lin, int col, const double& value)
{
  m_size.clamp(lin, col);

  m_buffer[index(lin, col)] = value;
}

void of::Image::setPixel(std::size_t i, const double& value)
{
  m_buffer[i] = value;
}

void of::Image::fill(const double& value)
{
  for(std::size_t i = 0; i < m_size.npixels; ++i)
    m_buffer[i] = value;
}

std::size_t of::Image::index(const std::size_t&lin, const std::size_t& col) const
{
  return lin * m_size.ncols + col;
}

of::Image* of::Image::filter2D(const Kernel& k) const
{
  Image* result = new Image(m_size, m_noDataValue);

  for(int lin = 0; lin < m_size.nlines; ++lin) // for each line
  {
    for(int col = 0; col < m_size.ncols; ++col) // for each column
    {
      double v = 0.0;

      // for each kernel pixel, do convolution
      for(int lw = -(int(k.height) / 2), lk = 0; lw <= int(k.height) / 2; ++lw, ++lk)
        for(int cw = -(int(k.width) / 2), ck = 0; cw <= int(k.width) / 2; ++cw, ++ck)
          v += k.get(lk, ck) * getPixel(lin, lw, col, cw);

      result->setPixel(lin, col, v);
    }
  }

  return result;
}

of::Image* of::Image::clone() const
{
  return new Image(*this);
}

int of::Image::reflect(const int& coord, const int& delta, const std::size_t& upper) const
{
  if(coord + delta < 0 || coord + delta >= upper)
    return coord - delta;
  else
    return coord + delta;
}
