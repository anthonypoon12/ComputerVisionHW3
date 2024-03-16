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

int applyFilter(Image image, int row, int col, vector<vector<int>> filter) {
  int sum = 0;
  for (int i = 0; i < filter.size(); i++) {
    for (int j = 0; j < filter[i].size(); j++) {
      int newRow = row + (i - filter.size() / 2);
      int newCol = col + (j - filter[i].size() / 2);
      if (isInBounds(image, newRow, newCol))
        sum += image.GetPixel(newRow, newCol) * filter[i][j];
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

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      int newValue1 = applyFilter(image, i, j, filter1);
      int newValue2 = applyFilter(image, i, j, filter2);
      newImage.SetPixel(i, j, newValue1*newValue1 + newValue2*newValue2);
      cout << i <<  " " << j << endl;
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
