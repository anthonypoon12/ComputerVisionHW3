//
// <YOUR NAME>
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

using namespace std;
using namespace ComputerVisionProjects;

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
  
  const int arrSize = sqrt(numRows*numRows + numCols*numCols);
  Image hough_image;
  hough_image.AllocateSpaceAndSetSize(arrSize,arrSize);
  hough_image.SetNumberGrayLevels(255);

  // vector<vector<double>> arr(arrSize, vector<double>(arrSize, 0.0));
  int maximum = 0;
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      double color = image.GetPixel(i, j);
      if (color > 0) {

        // max of theta is pi
        // max of rho is diagonal
        int x = i - numRows / 2;
        int y = j - numCols / 2;
        
        for (int z = 0; z < arrSize; z++) {
          double rho = x*cos(M_PI * z / arrSize) + y*sin(M_PI * z / arrSize);
          rho = rho + arrSize / 2;
          int newColor = hough_image.GetPixel(z, int(rho));
          hough_image.SetPixel(z, int(rho), newColor+1);
          if (maximum < newColor+1){
            // cout << maximum << " " << newColor << " " << newColor+1 << endl;
            cout << newColor << endl;
          }
          maximum = max(maximum, newColor+1);
        }
      }
    }
  }

  // Number of rows and columns in the hough_image
  size_t houghImgNumRows = hough_image.num_rows();
  size_t houghImgNumCols = hough_image.num_columns();

  for (int i = 0; i < houghImgNumRows; i++) {
    for (int j = 0; j < houghImgNumCols; j++) {
      double color = hough_image.GetPixel(i, j);
      hough_image.SetPixel(i, j, color);
      // hough_image.SetPixel(i, j, color * (255.0/maximum));
    }
  }

  if (!WriteImage(output_hough_image_filename, hough_image)){
    cout << "Can't write to file " << output_hough_image_filename << endl;
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
