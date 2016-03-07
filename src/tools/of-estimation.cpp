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
#include <string>

int main(int argc, char** argv)
{
  try
  {
    // Create the object that handles the input arguments
    TCLAP::CmdLine cmd("A tool to estimate optical flow given two consecutive images", ' ', "1.0.0");

    // Define image A path argument
    TCLAP::ValueArg<std::string> aPathArg("a", "imagea", "The path of the first image (A)", true, "", "string");

    // Define image B path argument
    TCLAP::ValueArg<std::string> bPathArg("b", "imageb", "The path of the second image (B)", true, "", "string");

    // Define output file path for (.flo) result file
    TCLAP::ValueArg<std::string> outputArg("o", "output", "Path to output file that will be created. The output file contains the coordinates of flow vectors", true, "", "string");

    // Add the arguments (keeping the best order for the user)
    cmd.add(outputArg);
    cmd.add(bPathArg);
    cmd.add(aPathArg);

    // Parse the given input parameters from agv array
    cmd.parse(argc, argv);

    // Everything is ok! Let's run the process

    // GDAL initialization
    GDALAllRegister();

    // Open input images
    GDALDataset* gDatasetA = (GDALDataset*)GDALOpen(aPathArg.getValue().c_str(), GA_ReadOnly);
    GDALDataset* gDatasetB = (GDALDataset*)GDALOpen(bPathArg.getValue().c_str(), GA_ReadOnly);

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

    // Save result as (.flo) file
    of->save(outputArg.getValue());

    delete of;
    delete imga;
    delete imgb;
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
