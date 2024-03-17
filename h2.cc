//
// Anthony
// Computational Vision Homework 3
// Program 2
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using namespace ComputerVisionProjects;



 // @brief Implementation of Task Program 2 to convert to binary image given a threshold.
 //        Same as p1 in Homework 2 -- you can copy the code
 // @param input_filename the name of the input image
 // @param threshold the threshold value
 // @param output_filename the name of the output image
void ConvertToBinary(const string &input_filename, int threshold, const string &output_filename) {
  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
    return;
  }

  // Number of rows and columns in the image
  size_t rows = image.num_rows();
  size_t cols = image.num_columns();

  // Sets pixel to black or white depending on its relation to the threshold
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){
      if (image.GetPixel(i, j) < threshold)
        image.SetPixel(i, j, 0);
      else
        image.SetPixel(i, j, 255);
    }
  }

  if (!WriteImage(output_filename, image)){
    cout << "Can't write to file " << output_filename << endl;
  }
}


int main(int argc, char **argv){  
  if (argc!=4) {
    printf("Usage: %s input_gray_edge_filename threshold output_binary_edge_filename\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const int threshold = atoi(argv[2]);
  const string output_filename(argv[3]);

  ConvertToBinary(input_filename, threshold, output_filename);

  return 0;
}
