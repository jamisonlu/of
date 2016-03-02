/*!
  \file src/Image.h
  \brief This class represents a single-band digital image.
  \author Douglas Uba
*/

#ifndef __OF_INTERNAL_IMAGE_H
#define __OF_INTERNAL_IMAGE_H

// STL
#include <limits>

namespace of
{
  // Define a value for 'not-informed no-data value'.
  const double NO_DATA_NOT_INFORMED = std::numeric_limits<double>::max();

  /*!
    \struct Size

    \brief Simple struct that defines the size of a two-dimensional image.
  */
  struct Size
  {
    /*! \brief Default constructor. */
    Size() : nlines(0), ncols(0), npixels(0) {}

    /*! \brief Constructor. */
    Size(std::size_t l, std::size_t c)
      : nlines(l), ncols(c), npixels(l * c) {}

    /*! Less than operator. */
    bool operator<(const Size& rhs) const
    {
      return nlines < rhs.nlines || ncols < rhs.ncols;
    }

    /*! Equal operator. */
    bool operator==(const Size& rhs) const
    {
      return nlines == rhs.nlines && ncols == rhs.ncols;
    }

    /*! Inequality operator. */
    bool operator!=(const Size& rhs) const
    {
      return !(*this == rhs);
    }

    /*! \brief  This methods limits the given position to size dimension. */
    void clamp(int& lin, int& col) const
    {
      if(lin < 0) lin = 0; else if(lin >= nlines) lin = nlines - 1;
      if(col < 0) col = 0; else if(col >= ncols)  col = ncols - 1;
    }

    bool isNull() const
    {
      return npixels == 0.0;
    }

    std::size_t nlines;  //!< Number of lines.
    std::size_t ncols;   //!< Number of columns.
    std::size_t npixels; //!< Number of pixels (nlines * ncols).
  };

  struct Kernel
  {
    /*! \brief Default constructor. */
    Kernel()
      : width(0), height(0), values(0)
    {
    }

    /*! \brief Constructor. */
    Kernel(std::size_t w, std::size_t h, double v = 0.0)
      : width(w), height(h)
    {
      values = new double[width * height];
      fill(v);
    }

    /*! \brief Constructor. */
    Kernel(std::size_t d, double v = 0.0)
      : width(d), height(d)
    {
      values = new double[width * height];
      fill(v);
    }

    /*! \brief Copy constructor. */
    Kernel(const Kernel& rhs)
      : width(rhs.width), height(rhs.height), values(0)
    {
      values = new double[width * height];
      for(std::size_t i = 0; i < width * height; ++i)
        values[i] = rhs.values[i];
    }

    /*! \brief Destructor. */
    ~Kernel()
    {
      delete [] values;
    }

    double get(std::size_t lin, std::size_t col) const
    {
      return values[lin * width + col];
    }

    void set(std::size_t lin, std::size_t col, double val)
    {
      values[lin * width + col] = val;
    }

    void fill(double v)
    {
      for(std::size_t i = 0; i < width * height; ++i)
        values[i] = v;
    }

    void mult(double f)
    {
      for(std::size_t i = 0; i < width * height; ++i)
        values[i] *= f;
    }

    Kernel& operator=(const Kernel& rhs)
    {
      if(this == &rhs)
        return *this;

      width = rhs.width;
      height = rhs.height;

      delete [] values;

      values = new double[width * height];

      for(std::size_t i = 0; i < width * height; ++i)
        values[i] = rhs.values[i];

      return *this;
    }

    std::size_t width;  //!< The kernel width.
    std::size_t height; //!< The kernel height.
    double* values;     //!< The kernel values.
  };

  /*!
    \class Image

    \brief This class represents a single-band digital image.
           Basically encapsulates a buffer of values and
           provides access methods to pixels.
  */
  class Image
  {
    public:

      /*!
        \brief Constructor.

        \param size The image size.
        \param noDataValue A value that represents 'no-data value'.
      */
      Image(const Size& size, double noDataValue = NO_DATA_NOT_INFORMED);

      /*!
        \brief Constructor.

        \param nlines The number of image lines.
        \param ncols  The number of image columns.
        \param noDataValue A value that represents 'no-data value'.
      */
      Image(std::size_t nlines, std::size_t ncols, double noDataValue = NO_DATA_NOT_INFORMED);

      /*!
        \brief Constructor.

        \param buffer The buffer of values.
        \param nlines The number of image lines.
        \param ncols  The number of image columns.
        \param noDataValue A value that represents 'no-data value'.

        \note The Image will take the ownership of the given buffer.
      */
      Image(double* buffer, std::size_t nlines, std::size_t ncols, double noDataValue = NO_DATA_NOT_INFORMED);

      /*!
        \brief Copy constructor.
      */
      Image(const Image& rhs);

      /*! \brief Destructor. */
      ~Image();

      /*!
        \brief This method returns the buffer values of image.

        \return The buffer values of image.
      */
      double* getBuffer() const;

      /*!
        \brief This method returns the image size.

        \return The image size.
      */
      const Size& getSize() const;

      /*!
        \brief This method returns the image number of lines.

        \return The image number of lines.
      */
      std::size_t getNLines() const;

      /*!
        \brief This method returns the image number of columns.

        \return The image number of columns.
      */
      std::size_t getNCols() const;

      /*!
        \brief This method returns the image number of pixels (nlines * ncols).

        \return The image number of pixels (nlines * ncols).
      */
      std::size_t getNPixels() const;

      /*!
        \brief This method returns the image 'no-data value'.

        \return The image 'no-data value'.
      */
      double getNoDataValue() const;

      /*!
        \brief This method returns if the pixel associated with line and column is 'no-data'.

        \param lin The line number.
        \param col The column number.

        \return True if the pixel is 'no-data'. False otherwise.
      */
      bool isNoData(int lin, int col) const;

      /*!
        \brief This method returns the pixel value associated with the given line and column.

        \param lin The line number.
        \param col The column number.

        \note The given line and column numbers will be clipped automatically to fit the image dimension.

        \return The pixel value associated with the given line and column.
      */
      double getPixel(int lin, int col) const;

      /*!
        \brief This method returns the pixel value associated with the given line, column and delta values.

        \param lin The line number.
        \param dl Delta line (lin + dl).
        \param col The column number.
        \param dc The delta column (col + dc).

        \note The reflect border strategy will be used.

        \return The pixel value associated with the given line, column and delta values.
      */
      double getPixel(int lin, int dl, int col, int dc) const;

      /*!
        \brief This method returns the pixel value associated with the given index.

        \param i The index number.

        \return The pixel value associated with the given line and column.
      */
      double getPixel(std::size_t i) const;

      /*!
        \brief This method sets the pixel value associated with the given line and column.

        \param lin The line number.
        \param col The column number.
        \param value The pixel value.

        \note The given line and column numbers will be clipped automatically to fit the image dimension.
      */
      void setPixel(int lin, int col, const double& value);

      /*!
        \brief This method sets the pixel value associated with the given index.

        \param i The index number.
        \param value The pixel value.
      */
      void setPixel(std::size_t i, const double& value);

      /*!
        \brief This method fills the image with the given value.

        \param value The value that will be used to fill the image.
      */
      void fill(const double& value);

      /*!
        \brief This method returns the buffer index associated with the given line and column.

        \param lin The line number.
        \param col The column number.

        \return The buffer index.
      */
      std::size_t index(const std::size_t& lin, const std::size_t& col) const;

      /*!
        \brief This method applies a filter 2D to this image using the given kernel.

        \param kernel The kernel.

        \return A new image filtered.
      */
      Image* filter2D(const Kernel& kernel) const;

      /*!
        \brief Clone method.
      */
      Image* clone() const;

    private:

      /*! \brief This method implements the reflect border strategy. */
      int reflect(const int& coord, const int& delta, const std::size_t& upper) const;

    private:

      double* m_buffer;      //!< Buffer of values (i.e. pixels).
      Size m_size;           //!< The image size.
      double m_noDataValue;  //!< A value that represents 'no-data value'.
  };

} // end namespace of

#endif // __OF_INTERNAL_IMAGE_H
