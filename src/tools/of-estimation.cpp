/*!
  \file tools/of-estimation.cpp
  \brief Optical Flow Estimation Command Line Tool.
  \author Douglas Uba
*/

// Optical Flow
#include "../of/Exception.h"
#include "../of/Image.h"
#include "../of/LucasKanadeC2F.h"

// GDAL/OGR
#include <gdal_priv.h>

// TCLAP
#include <tclap/CmdLine.h>

// STL
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void Tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiter)
{
  std::size_t lastpos = str.find_first_not_of(delimiter, 0);

  std::size_t pos = str.find_first_of(delimiter, lastpos);

  while(std::string::npos != pos || std::string::npos != lastpos)
  {
    tokens.push_back(str.substr(lastpos, pos - lastpos));
    lastpos = str.find_first_not_of(delimiter, pos);
    pos = str.find_first_of(delimiter, lastpos);
  }
}

std::string Convert2String(std::size_t i)
{
  std::ostringstream strs; strs << i;
  return strs.str();
}

int main(int argc, char** argv)
{
  try
  {
    // Create the object that handles the input arguments
    TCLAP::CmdLine cmd("A tool to estimate optical flow given a set of consecutive images", ' ', "1.0.0");

    // Define images path argument
    TCLAP::ValueArg<std::string> imagesPathArg("i", "images", "Paths of consecutive images comma separated. e.g. path-1,path-2,path-n", true, "", "string");

    // Define output directory argument
    TCLAP::ValueArg<std::string> outputDirArg("o", "output", "Path to output directory that will contain the results. \
                                                              Each output file (.flo) contains the coordinates of flow vectors at instant t+1",
                                                              true, "", "string");

    // Add the arguments
    cmd.add(outputDirArg);
    cmd.add(imagesPathArg);

    // Parse the given input parameters from agv array
    cmd.parse(argc, argv);

    // Get the set of image paths separately
    std::vector<std::string> paths;
    Tokenize(imagesPathArg.getValue(), paths, ",");

    if(paths.size() < 2)
      throw of::Exception("Wrong parameter 'images': inform at least two consecutive images");

    // Everything is ok! Let's run the process

    // GDAL initialization
    GDALAllRegister();

    std::cout << "Processing..." << std::endl;

    for(std::size_t i = 0; i < paths.size() - 1; ++i) // for each image pair
    {
      std::cout << "- Image A: " << paths[i] << std::endl;
      std::cout << "- Image B: " << paths[i + 1] << std::endl;

      // Open input images
      GDALDataset* gDatasetA = (GDALDataset*)GDALOpen(paths[i].c_str(), GA_ReadOnly);
      GDALDataset* gDatasetB = (GDALDataset*)GDALOpen(paths[i + 1].c_str(), GA_ReadOnly);

      // Retrieve dimensions
      std::size_t nlines = gDatasetA->GetRasterYSize();
      std::size_t ncols = gDatasetA->GetRasterXSize();
      std::size_t size = nlines * ncols;

      // Read buffers
      double* buffera = new double[size];
      double* bufferb = new double[size];
      gDatasetA->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, ncols, nlines, buffera, ncols, nlines, GDT_Float64, 0, 0);
      gDatasetB->GetRasterBand(1)->RasterIO(GF_Read, 0, 0, ncols, nlines, bufferb, ncols, nlines, GDT_Float64, 0, 0);

      // Close! GDAL is used only to read image data
      GDALClose(gDatasetA);
      GDALClose(gDatasetB);

      // Encapsulate buffers
      of::Image* imga = new of::Image(buffera, nlines, ncols);
      of::Image* imgb = new of::Image(bufferb, nlines, ncols);

      // Execute!
      of::OpticalFlow* of = new of::LucasKanadeC2F(imga, imgb);
      of->compute();

      std::string uvfile = outputDirArg.getValue() + "uv-" + Convert2String(i + 1) + ".flo";

      std::cout << "- Result file (u,v): " << uvfile << std::endl;

      // Save result as (.flo) file
      of->save(uvfile);

      delete of;
      delete imga;
      delete imgb;
    }
  }
  catch(TCLAP::ArgException& e)
  {
    std::cerr << std::endl << "Argument exception: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(const of::Exception& e)
  {
    std::cerr << std::endl << "An exception has occurred: " << e.what() << std::endl;

    return EXIT_FAILURE;
  }
  catch(...)
  {
    std::cerr << std::endl << "An unexpected exception has occurred!" << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
