/*!
  \file src/HornSchunck.h

  \brief This class implements the Horn & Schunck method for estimating optical flow.
         Reference: B.K.P. Horn and B.G. Schunck, "Determining optical flow." Artificial Intelligence, vol 17, pp 185–203, 1981
                    http://dspace.mit.edu/handle/1721.1/6337

  \author Douglas Uba
*/

#include "Config.h"
#include "HornSchunck.h"
#include "Image.h"

of::HornSchunck::HornSchunck(Image* a, Image* b)
  : OpticalFlow(a, b),
    m_alpha(15),
    m_maxIterations(std::string::npos),
    m_e(DEFAULT_HS_AUTO_STOP_THRESHOLD)
{
}

of::HornSchunck::~HornSchunck()
{
}

void of::HornSchunck::compute()
{
  initialize();

  // Compute derivative images (fx, fy and ft)
  computeDerivativeImages();

  std::size_t size = m_u->getNPixels();

  // Local averages
  double* ubar = new double[size];
  double* vbar = new double[size];

  // Classical Horn-Schunck method iterations
  for(std::size_t it = 0; it < m_maxIterations; ++it)
  {
    computeLocalAvg(ubar, m_u);
    computeLocalAvg(vbar, m_v);

    double sc = 0.0;

     // Compute coordinates
    for(std::size_t i = 0; i < size; ++i)
    {
      double t = m_fx->getPixel(i) * ubar[i] + m_fy->getPixel(i) * vbar[i] + m_ft->getPixel(i);
      t /= m_alpha * m_alpha + m_fx->getPixel(i) * m_fx->getPixel(i) + m_fy->getPixel(i) * m_fy->getPixel(i);

      double u = ubar[i] - m_fx->getPixel(i) * t;
      double v = vbar[i] - m_fy->getPixel(i) * t;

      sc += (u - m_u->getPixel(i)) * (u - m_u->getPixel(i)) + (v - m_v->getPixel(i)) * (v - m_v->getPixel(i));

      m_u->setPixel(i, u);
      m_v->setPixel(i, v);
    }

    // Can stop?
    if(sc / size <= m_e * m_e)
      break;
  }

  delete [] ubar;
  delete [] vbar;
}

void of::HornSchunck::setAlpha(double alpha)
{
  m_alpha = alpha;
}

void of::HornSchunck::setMaxNumberOfIterations(std::size_t n)
{
  m_maxIterations = n;
}

void  of::HornSchunck::setAutoStopThreshold(double e)
{
  m_e = e;
}

void of::HornSchunck::computeLocalAvg(double* avg, Image* coords)
{
  std::size_t i = 0;
  for(int lin = 0; lin < coords->getNLines(); ++lin)
  {
    for(int col = 0; col < coords->getNCols(); ++col)
    {
      avg[i++] = (1.0 / 6.0) * (coords->getPixel(lin, col - 1) + coords->getPixel(lin, col + 1)
        + coords->getPixel(lin - 1, col) + coords->getPixel( lin + 1, col)) +
        (1.0 / 12.0) * (coords->getPixel(lin - 1, col - 1)
        + coords->getPixel(lin - 1, col + 1)
        + coords->getPixel(lin + 1, col - 1)
        + coords->getPixel(lin + 1, col + 1));
    }
  }
}
