/*!
  \file src/of/Exception.h
  \brief This class is designed to declare objects to be thrown as exceptions.
  \author Douglas Uba
*/

#include "Exception.h"

// STL
#include <ostream>

of::Exception::Exception() throw()
{
}

of::Exception::Exception(const std::string& what) throw()
  : m_msg(what)
{
}

of::Exception::Exception(const char* const what) throw()
  : m_msg(what)
{
}

of::Exception::~Exception() throw()
{
}

const char* of::Exception::what() const throw()
{
  return m_msg.c_str();
}

const char* of::Exception::getClassName() const throw()
{
  return "of::Exception";
}

std::ostream& operator<<(const of::Exception& e, std::ostream& o)
{
  o << e.getClassName() << ": " << e.what();

  return o;
}
