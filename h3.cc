//
// Anthony Poon
// Computational Vision Homework 3
// Program 3
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;
using namespace ComputerVisionProjects;

 // @brief Implementation of Bound checking function for a given pixel
 //  
 // @param image the Image object
 // @param row the row coordinate of the target pixel
 // @param col the column coordinate of the target pixel
bool isInBounds(Image *image, int row, int col) {
  // Number of rows and columns in the image
  size_t numRows = image->num_rows();
  size_t numCols = image->num_columns();

  if (row < 0 || row >= numRows)
    return false;
  if (col < 0 || col >= numCols)
    return false;
  return true;
}

// @brief Computes Hough Transform
// @param input_filename the filename of the input binary edge image
// @param output_hough_image_filename the name of the output hough image (gray scale pgm)
// @param output_voting_array_filename the name of the output hough voting array
void ComputeHoughTransform(const string &input_filename, const string & output_hough_image_filename, const string &output_voting_array_filename) {
  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
    return;
  }


  // Number of rows and columns in the image
  size_t numRows = image.num_rows();
  size_t numCols = image.num_columns();
  
  const double thetaResolution = 1.0;
  double rhoResolution = 0.5;

  const int rhoRange = sqrt(numRows*numRows + numCols*numCols);
  const int thetaWidth = 180/thetaResolution;
  const int arrWidth = rhoRange/rhoResolution;

  vector<vector<int>> arr(thetaWidth, vector<int>(arrWidth, 0));
  int maximum = 0;

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      double color = image.GetPixel(i, j);
      if (color > 0) {
        for (int z = 0; z < thetaWidth; z++) {
          double theta = M_PI * z / thetaWidth;
          double rho = i*cos(theta) + j*sin(theta);
          int k = int(rho/rhoRange * arrWidth);
          if (k >= 0 && k < arrWidth){
            arr[z][k]++;
            maximum = max(maximum, arr[z][k]);
          }
        }
      }
    }
  }


  Image hough_image;
  hough_image.AllocateSpaceAndSetSize(thetaWidth,arrWidth);
  hough_image.SetNumberGrayLevels(255);

  for (int i = 0; i < thetaWidth; i++) {
    for (int j = 0; j < arrWidth; j++) {
      hough_image.SetPixel(i, j, arr[i][j] * 255/maximum);
    }
  }

  if (!WriteImage(output_hough_image_filename, hough_image)){
    cout << "Can't write to file " << output_hough_image_filename << endl;
  }
ofstream file(output_voting_array_filename);
if (file.is_open())
  {
    for(int i = 0; i < arr.size(); i++){
      for (int j = 0; j < arr[i].size(); j++) {
        file << arr[i][j] << " " ;
      }
      file << "\n";
    }
    file.close();
  }
}


int main(int argc, char **argv){
  
  if (argc!=4) {
    printf("Usage: %s input_binary_edge_filename output_gray_legel_hough_image_filename output_hough_voting_array(txt)\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string output_hough_image_filename(argv[2]);
  const string output_voting_array_filename(argv[3]);

  ComputeHoughTransform(input_filename, output_hough_image_filename, output_voting_array_filename);
  
  return 0;
}
