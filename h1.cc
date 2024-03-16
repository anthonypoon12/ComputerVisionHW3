//
// Anthony Poon
// Computational Vision Homework 3
// Program 1
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;
using namespace ComputerVisionProjects;

bool isInBounds(Image image, int row, int col) {
  // Number of rows and columns in the image
  size_t numRows = image.num_rows();
  size_t numCols = image.num_columns();

  if (row < 0 || row >= numRows)
    return false;
  if (col < 0 || col >= numCols)
    return false;
  return true;
}

 // @brief Implementation of Task Program 1 to produce an edge gray-level image
 //  
 // @param input_filename the name of the input image
 // @param output_filename the name of the gray-level output edge image
void ComputeEdgeImage(const string &input_filename, const string &output_filename) {
  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
    return;
  }

  // Image newImage = image;

  // Number of rows and columns in the image
  size_t numRows = image.num_rows();
  size_t numCols = image.num_columns();
  vector<vector<int>> filter1 = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
  };
  vector<vector<int>> filter2 = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
  };

  vector<double> allValues = {};

  for (int i = 1; i < numRows - 1; i++) {
    for (int j = 1; j < numCols - 1; j++) {

      int newValue1 = 0;
      int newValue2 = 0;

      for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++){
          int newPixel = image.GetPixel(i + x, j + y);
          newValue1 += filter1[x + 1][y + 1] * newPixel;
          newValue2 += filter2[x + 1][y + 1] * newPixel;
        }
      }

      double gradient_magnitude = sqrt(newValue1*newValue1 + newValue2*newValue2);
      allValues.push_back(gradient_magnitude);
    }
  }


  double min_val = *std::min_element(allValues.begin(), allValues.end());
  double max_val = *std::max_element(allValues.begin(), allValues.end());

  for (double& value : allValues) {
      value = (value - min_val) / (max_val - min_val);
  }


  for (int i = 1; i < numRows - 1; i++) {
    for (int j = 1; j < numCols - 1; j++) {
        double value = allValues[0];
        allValues.erase(allValues.begin()); // Erase the first element

        image.SetPixel(i, j, value * 255);
    }
  }

  // if (!WriteImage(output_filename, newImage)){
  if (!WriteImage(output_filename, image)){
    cout << "Can't write to file " << output_filename << endl;
  }
}

int main(int argc, char **argv){  
  if (argc!=3) {
    printf("Usage: %s input_gray_level_file_name output_edge_image_file_name\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string output_filename(argv[2]);

  ComputeEdgeImage(input_filename, output_filename);

  return 0;
}
