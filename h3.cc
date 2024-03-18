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

void setSquare(Image *image, int row, int col, int color, int resolution) {
  for (int i = -1 * resolution/2; i < resolution/2; i++) {
    for (int j = -1 * resolution/2; j < resolution/2; j++) {
      
      if (isInBounds(image, row + i, col + j)){
        int currentColor = image->GetPixel(row + i, col + j);
        if (currentColor < color) {
          image->SetPixel(row+i, col+j, color);
        }
      }
    }
  }
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
  
  const int rhoRange = sqrt(numRows*numRows + numCols*numCols);
  const int thetaWidth = rhoRange;

  vector<vector<int>> arr(thetaWidth, vector<int>(rhoRange, 0));
  int maximum = 0;

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      double color = image.GetPixel(i, j);
      if (color > 0) {

        // max of theta is pi
        // max of rho is diagonal
        int x = i - numRows / 2;
        int y = j - numCols / 2;
        
        for (int z = 0; z < thetaWidth; z++) {
          double theta = M_PI * z / thetaWidth;
          double rho = x*cos(theta) + y*sin(theta);
          rho = rho + rhoRange / 2;
          arr[z][int(rho)]++;
          maximum = max(maximum, arr[z][int(rho)]);
        }
      }
    }
  }


  Image hough_image;
  hough_image.AllocateSpaceAndSetSize(thetaWidth,rhoRange);
  hough_image.SetNumberGrayLevels(255);

  vector<vector<int>> newArr(thetaWidth/3, vector<int>(rhoRange/3, 0));

  for (int i = 0; i < thetaWidth; i++) {
    for (int j = 0; j < rhoRange; j++) {
      setSquare(&hough_image, i, j, arr[i][j] * 255/maximum, 10);
      if (i/3 < newArr.size() && j/3 < newArr[0].size())
        newArr[i/3][j/3] = hough_image.GetPixel(i, j);
    }
  }

  if (!WriteImage(output_hough_image_filename, hough_image)){
    cout << "Can't write to file " << output_hough_image_filename << endl;
  }
ofstream file(output_voting_array_filename);
if (file.is_open())
  {
    for(int i = 0; i < newArr.size(); i++){
      for (int j = 0; j < newArr[i].size(); j++) {
        file << newArr[i][j] << " " ;
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
