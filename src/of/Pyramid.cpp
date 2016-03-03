/*!
  \file src/of/Pyramid.cpp
  \brief This class represents an image hierarchical pyramid.
  \author Douglas Uba
*/

#include "Image.h"
#include "Pyramid.h"

// STL
#include <algorithm>
#include <cassert>

// Static members declarations
of::Kernel of::Pyramid::sm_gkDown;
of::Kernel of::Pyramid::sm_gkUp;
of::Pyramid::Initializer of::Pyramid::sm_initializer;

of::Pyramid::Pyramid(Image* image, std::size_t nLevels)
{
  m_pyramid.resize(nLevels + 1);

  // Add the first level (i.e. the original image)
  m_pyramid[0] = image->clone();

  build();
}

of::Pyramid::~Pyramid()
{
  for(std::size_t i = 0; i < m_pyramid.size(); ++i)
    delete m_pyramid[i];
}

const std::vector<of::Image*>& of::Pyramid::getLevels() const
{
  return m_pyramid;
}

of::Image* of::Pyramid::getLevel(std::size_t i) const
{
  assert(i < m_pyramid.size());
  return m_pyramid[i];
}

std::size_t of::Pyramid::getNLevels() const
{
  return m_pyramid.size();
}

void of::Pyramid::updateLevel(std::size_t i, Image* image)
{
  assert(i < m_pyramid.size());

  delete m_pyramid[i];

  m_pyramid[i] = image;
}

of::Image* of::Pyramid::down(Image* image)
{
  // Apply gaussian kernel for downsampling
  Image* gaussian = image->filter2D(sm_gkDown);

  // Create the requested level
  Image* level = new Image(image->getNLines() * 0.5, image->getNCols() * 0.5);

  // Remove even-numbered line and column from gaussian
  for(std::size_t lin = 0, oddlin = 1; lin < level->getNLines(); ++lin, oddlin += 2)
    for(std::size_t col = 0, oddcol = 1; col < level->getNCols(); ++col, oddcol += 2)
      level->setPixel(lin, col, gaussian->getPixel(oddlin, oddcol));

  delete gaussian;

  return level;
}

of::Image* of::Pyramid::up(Image* image, const Size& size)
{
  Size isize = size;

  if(size.isNull())
    isize = Size(image->getNLines() * 2.0, image->getNCols() * 2.0);

  // Create the requested level
  Image* level = new Image(isize);

  // Even line and column filled with zeros
  level->fill(0.0);

  // Fill odd line and column
  for(std::size_t lin = 1, prevlin = 0; lin < level->getNLines(); lin += 2, ++prevlin)
    for(std::size_t col = 1, prevcol = 0; col < level->getNCols(); col += 2, ++prevcol)
      level->setPixel(lin, col, image->getPixel(prevlin, prevcol));

  // Apply gaussian kernel for upsampling
  Image* gaussian = level->filter2D(sm_gkUp);

  delete level;

  return gaussian;
}

std::size_t of::Pyramid::getMaxNumberOfLevels(Image* image)
{
  std::size_t levels = 0;
  std::size_t size = std::min(image->getNLines(), image->getNCols()) * 0.5;

  while(size >= 2)
  {
    ++levels;
    size *= 0.5;
  }

  return levels;
}

void of::Pyramid::build()
{
  for(std::size_t i = 1; i < m_pyramid.size(); ++i)
    m_pyramid[i] = down(m_pyramid[i - 1]);
}

of::Pyramid::Initializer::Initializer()
{
  // Gaussian kernel used on downsampling proces
  sm_gkDown = Kernel(5, 0.0);
  sm_gkDown.set(0, 0, 1.0);
  sm_gkDown.set(0, 1, 4.0);
  sm_gkDown.set(0, 2, 6.0);
  sm_gkDown.set(0, 3, 4.0);
  sm_gkDown.set(0, 4, 1.0);
  sm_gkDown.set(1, 0, 4.0);
  sm_gkDown.set(1, 1, 16.0);
  sm_gkDown.set(1, 2, 24.0);
  sm_gkDown.set(1, 3, 16.0);
  sm_gkDown.set(1, 4, 4.0);
  sm_gkDown.set(2, 0, 6.0);
  sm_gkDown.set(2, 1, 24.0);
  sm_gkDown.set(2, 2, 36.0);
  sm_gkDown.set(2, 3, 24.0);
  sm_gkDown.set(2, 4, 6.0);
  sm_gkDown.set(3, 0, 4.0);
  sm_gkDown.set(3, 1, 16.0);
  sm_gkDown.set(3, 2, 24.0);
  sm_gkDown.set(3, 3, 16.0);
  sm_gkDown.set(3, 4, 4.0);
  sm_gkDown.set(4, 0, 1.0);
  sm_gkDown.set(4, 1, 4.0);
  sm_gkDown.set(4, 2, 6.0);
  sm_gkDown.set(4, 3, 4.0);
  sm_gkDown.set(4, 4, 1.0);

  // Gaussian kernel used on upsampling proces
  sm_gkUp = Kernel(sm_gkDown);

  // Adjust factors
  sm_gkDown.mult(1.0 / 256.0);
  sm_gkUp.mult(1.0 / 64.0);
}
