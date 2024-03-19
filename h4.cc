//
// Anthony Poon
// Computational Vision Homework 3
// Program 4 
//

#include "image.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include "h4.h"

using namespace std;
using namespace ComputerVisionProjects;

// @brief Computes and draws lines from Hough transform
// @param input_filename the filename of the input original image
// @param input_voting_array_filename the filename of the input hough voting array (from h3)
// @param threshold is the threshold for detection of peaks in hough voting array
// @param output_gray_level_line_filename the filename of the output line image
void ComputeAndDrawLinesFromHough(const string &input_filename, const string &input_voting_array_filename, int threshold, const string &output_gray_level_line_filename) {
  Image image;
  if (!ReadImage(input_filename, &image)) {
    cout <<"Can't open file " << input_filename << endl;
    return;
  }

  ifstream inputFile(input_voting_array_filename);
  if (!inputFile) {
        cerr << "Error opening file." << endl;
        return;
  }

  vector<vector<int>> array2D;

  string line;
  while (getline(inputFile, line)) {
    istringstream iss(line);
    int num;
    vector<int> row;
    while (iss >> num) {
        row.push_back(num);
    }
    array2D.push_back(row);
  }
  // Number of rows and columns in the image
  size_t numRows = image.num_rows();
  size_t numCols = image.num_columns();

  // Make the image binary
  for (int i = 0; i < array2D.size(); i++) {
    for (int j = 0; j < array2D[i].size(); j++) {
      array2D[i][j] = array2D[i][j] >= threshold ? 255 : 0;
    }
  }

  if (!WriteImage(output_gray_level_line_filename, image)){
    cout << "Can't write to file " << output_gray_level_line_filename << endl;
  }
}

void drawLineForArrCoord(int i, std::vector<std::vector<int>> &array2D, int j, size_t numRows, size_t numCols, ComputerVisionProjects::Image &image)
{
  vector<double> points;
  double theta = i * M_PI / array2D.size();
  double rho = j * sqrt(numRows * numRows + numCols * numCols) / array2D[i].size();

  double x0 = 0;
  double y0 = (rho) / sin(theta);
  if (sin(theta) == 0)
  {
    y0 = numCols - 1;
  }

  if (y0 >= 0 && y0 < numCols)
  {
    points.push_back(x0);
    points.push_back(y0);
  }

  double y1 = 0;
  double x1 = (rho) / cos(theta);
  if (cos(theta) == 0)
  {
    x1 = numRows - 1;
  }

  if (x1 >= 0 && x1 < numRows)
  {
    points.push_back(x1);
    points.push_back(y1);
  }

  x0 = numRows - 1;
  y0 = (rho - (x0 * cos(theta))) / sin(theta);
  if (sin(theta) == 0)
  {
    y0 = numCols - 1;
  }

  if (y0 >= 0 && y0 < numCols)
  {
    points.push_back(x0);
    points.push_back(y0);
  }

  y1 = numCols - 1;
  x1 = (rho - (y1 * sin(theta))) / cos(theta);
  if (cos(theta) == 0)
  {
    x1 = numRows - 1;
  }

  if (x1 >= 0 && x1 < numRows)
  {
    points.push_back(x1);
    points.push_back(y1);
  }

  DrawLine(int(points[0]), int(points[1]), int(points[2]), int(points[3]), 255, &image);
}
int main(int argc, char **argv){
  
  if (argc!=5) {
    printf("Usage: %s {input original gray-level image} {input hough voting array} {input hough threshold value} {output gray-level line image}\n", argv[0]);
    return 0;
  }
  
  const string input_filename(argv[1]);
  const string input_voting_array_filename(argv[2]);
  const int threshold = atoi(argv[3]);
  const string output_gray_level_line_filename(argv[4]);

  ComputeAndDrawLinesFromHough(input_filename, input_voting_array_filename, threshold, output_gray_level_line_filename);
  
  return 0;
}
