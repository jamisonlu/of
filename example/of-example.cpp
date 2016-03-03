/*!
  \file example/of-example.cpp
  \brief Optical Flow Algorithms Example.
  \author Douglas Uba
*/

// Optical Flow
#include <of/HornSchunck.h>
#include <of/Image.h>
#include <of/LucasKanade.h>
#include <of/LucasKanadeC2F.h>

// GDAL/OGR
#include <gdal_priv.h>

// STL
#include <iostream>
#include <string>

// Auxiliary function to create an output image
GDALDataset* CreateImage(const std::string& path,
                         std::size_t nlines, std::size_t ncols, std::size_t nBands,
                         GDALDataType type, const std::string& driver)
{
  return GetGDALDriverManager()->GetDriverByName(driver.c_str())->Create(path.c_str(), ncols, nlines, nBands, type, 0);
}

// Auxiliary function to export optical flow results
void ExportResults(of::OpticalFlow* of, const std::string& dir)
{
  // Export (u,v) .flo file
  of->save(dir + "uv.flo");

  // Export derivative images
  of::Image* fx = of->getFx();
  of::Image* fy = of->getFy();
  of::Image* ft = of->getFt();

  std::size_t nlines = fx->getNLines();
  std::size_t ncols = fx->getNCols();

  GDALDataset* derivatives = CreateImage(dir + "derivatives.tif", nlines, ncols, 3, GDT_Float64, "GTiff");
  derivatives->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, ncols, nlines, fx->getBuffer(), ncols, nlines, GDT_Float64, 0, 0);
  derivatives->GetRasterBand(2)->RasterIO(GF_Write, 0, 0, ncols, nlines, fy->getBuffer(), ncols, nlines, GDT_Float64, 0, 0);
  derivatives->GetRasterBand(3)->RasterIO(GF_Write, 0, 0, ncols, nlines, ft->getBuffer(), ncols, nlines, GDT_Float64, 0, 0);

  GDALClose(derivatives);

  // Export warped image
  GDALDataset* warped = CreateImage(dir + "warp.tif", nlines, ncols, 1, GDT_Float64, "GTiff");
  warped->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, ncols, nlines, of->getWarped()->getBuffer(), ncols, nlines, GDT_Float64, 0, 0);
  GDALClose(warped);

  // Export error image
  GDALDataset* error = CreateImage(dir + "error.tif", nlines, ncols, 1, GDT_Float64, "GTiff");
  error->GetRasterBand(1)->RasterIO(GF_Write, 0, 0, ncols, nlines, of->getError()->getBuffer(), ncols, nlines, GDT_Float64, 0, 0);
  GDALClose(error);
}

int main()
{
  std::cout << ":: Optical Flow Algorithms ::" << std::endl;

  // GDAL initialization
  GDALAllRegister();

  std::string basedir = "D:/development/cpp/of/data/input/";

  // Open images
  GDALDataset* gDatasetA = (GDALDataset*)GDALOpen((basedir + "satellitea.jpg").c_str(), GA_ReadOnly);
  GDALDataset* gDatasetB = (GDALDataset*)GDALOpen((basedir + "satelliteb.jpg").c_str(), GA_ReadOnly);

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

  //of::HornSchunck hs(imga, imgb);
  //hs.compute();
  //ExportResults(&hs, basedir + "hs/");

  //of::LucasKanade lk(imga, imgb);
  //lk.compute();
  //ExportResults(&lk, basedir + "lk/");

  of::LucasKanadeC2F lkc2f(imga, imgb);
  lkc2f.compute();
  ExportResults(&lkc2f, basedir + "lkc2f/");

  delete imga;
  delete imgb;

  std::cout << "Press enter to exit...";
  std::cin.get();

  return 0;
}
