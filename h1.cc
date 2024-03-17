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

 // @brief Implementation of function that applies convolutional filter to iamge
 //  
 // @param image the Image object
 // @param row the row coordinate of the target pixel
 // @param col the column coordinate of the target pixel
 // @param filter the square filter we use to apply filtering
int applyFilter(Image *image, int row, int col, vector<vector<int>> filter) {
  int sum = 0;
  // Iterate through each pixel in the filter
  for (int i = 0; i < filter.size(); i++) {
    for (int j = 0; j < filter[i].size(); j++) {
      // The target pixel is at the center of the filter
      // This offsets the pixels correctly
      int newRow = row + (i - filter.size() / 2);
      int newCol = col + (j - filter[i].size() / 2);

      // If the pixel that is filtered is not in bounds, we treat it as black
      if (isInBounds(image, newRow, newCol))
        sum += image->GetPixel(newRow, newCol) * filter[i][j];
    }
  }
  return sum;
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

  Image newImage = image;

  // Number of rows and columns in the image
  size_t numRows = image.num_rows();
  size_t numCols = image.num_columns();

  // First derivative in relation to x
  vector<vector<int>> X_filter = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
  };

  // First derivative in relation to y
  vector<vector<int>> Y_filter = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
  };

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      int gx = applyFilter(&image, i, j, X_filter);
      int gy = applyFilter(&image, i, j, Y_filter);
      double magnitude = sqrt(gx*gx + gy*gy);
      newImage.SetPixel(i, j, magnitude);
    }
  }

  if (!WriteImage(output_filename, newImage)){
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