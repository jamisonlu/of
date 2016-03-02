/*!
  \file src/Exception.h
  \brief This class is designed to declare objects to be thrown as exceptions.
  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_EXCEPTION_H
#define __OF_INTERNAL_EXCEPTION_H

// STL
#include <exception>
#include <string>

namespace of
{
    /*!
      \class Exception

      \brief This class is designed to declare objects to be thrown as exceptions.

      \note Exception class is a subclass of std::exception.
    */
    class Exception : public virtual std::exception
    {
      public:

        /*! \brief Default constructor. */
        Exception() throw();

        /*!
          \brief It initializes a new Exception.

          \param what A brief description of what has raised the exception.
        */
        explicit Exception(const std::string& what) throw();

        /*!
          \brief It initializes a new Exception.

          \param what A brief description of what has raised the exception.
        */
        explicit Exception(const char* const what) throw();

        /*! \brief Destructor. */
        virtual ~Exception() throw();

        /*!
          \brief It outputs the exception message.

          \return The exception message.
        */
        virtual const char* what() const throw();

        /*!
          \brief It return the exception class name.

          \return the exception class name.
        */
        virtual const char* getClassName() const throw();

      protected:

        std::string m_msg; //!< The internal exception message.
    };

    /*!
      \brief It serializes the exception and sends it to the output stream.

      \param e An exception object.
      \param o An output stream.

      \return The output stream.
    */
    std::ostream& operator<<(const Exception& e, std::ostream& o);

} // end namespace of

#endif  // __OF_INTERNAL_EXCEPTION_H
