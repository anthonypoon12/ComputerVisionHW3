//
// Anthony Poon
// Computational Vision Homework 3
// Program 4 
//

#include "image.h"
#include "DisjSets.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <unordered_map>
#include <algorithm>

using namespace std;
using namespace ComputerVisionProjects;

bool isInBounds(vector<vector<int>> array2D, int row, int col){
  // number of rows and columns
  size_t max_row = array2D.size();
  size_t max_col = array2D[0].size();
  if (row < 0 || row >= max_row)
    return false;
  if (col < 0 || col >= max_col)
    return false;
  return true;
}

void firstPass(vector<vector<int>> &array2D, DisjSets &sets, int &numberOfLabels) {
  // number of rows and columns
  size_t rows = array2D.size();
  size_t cols = array2D[0].size();

  // Keeps track of "new" object that we encounter
  int labelNumber = 1;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {

      // Current pixel
      int a = array2D[i][j];

      // If the pixel is not the background
      if (a != 0) {
        // Label that we will assign to current pixel
        int newLabel;

        //  Check 3 other neighbors in 2x2 square
        int b = isInBounds(array2D, i, j - 1) ? array2D[i][j - 1] : 0;
        int c = isInBounds(array2D, i - 1, j) ? array2D[i - 1][j] : 0;
        int d = isInBounds(array2D, i - 1, j - 1) ? array2D[i - 1][j - 1] : 0;
      
        // If neighbors are all background, we have a new label
        if (b == 0 && c == 0 && d == 0) {
          newLabel = labelNumber;
          labelNumber++;
          numberOfLabels++;
        }

        else {
          // If diagonal is an object, current pixel is same object
          if (d != 0) {
            newLabel = d;
          }

          else {
            // If both b and c are objects, pick arbitrarily and union them
            if (b != 0 && c != 0) {
              int root1 = sets.find(b);
              int root2 = sets.find(c);
              if (root1 != root2){
                sets.unionSets(root1, root2);
                numberOfLabels--;
              }
              newLabel = b;
            }

            // If there is only one non-zero neighbor, take that value
            else if (b != 0) {
              newLabel = b;
            }
            else {
              newLabel = c;
            }
          }
        }
      
        // Record label in pixel of image
        array2D[i][j] = newLabel;
      }
    }
  }
}

void secondPass(vector<vector<int>> &array2D, DisjSets &sets, int &numberOfLabels) {
  // number of rows and columns
  size_t rows = array2D.size();
  size_t cols = array2D[0].size();

  // Assign integer starting from 1 to each object label for coloring
  unordered_map<int, int> labelCounts = {};
  int labelCounter = 1;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {

      // Finds true label of pixel in equivalency table
      int label = sets.find(array2D[i][j]);

      if (label != 0){
        array2D[i][j] = label;
      }
    }
  }
}

void PerformSequentialLabeling(vector<vector<int>> &array2D) {

// number of rows and columns
  size_t rows = array2D.size();
  size_t cols = array2D[0].size();

  int numberOfLabels = 0;
  DisjSets sets = DisjSets(rows * cols);

  firstPass(array2D, sets, numberOfLabels);

  secondPass(array2D, sets, numberOfLabels);

}

void populateSumsAndArea(vector<vector<int>> &array2D, unordered_map<int, vector<double>> &labelMap,vector<vector<int>> &originalArr) {
  // number of rows and columns
  size_t rows = array2D.size();
  size_t cols = array2D[0].size();

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int pixelColor = array2D[i][j];

      if (pixelColor != 0){
        if (labelMap.find(pixelColor) == labelMap.end()) {
          labelMap[pixelColor] = vector<double>(5);
        }
        labelMap[pixelColor][0] += double(i)*originalArr[i][j]; // sum of row value
        labelMap[pixelColor][1] += double(j)*originalArr[i][j]; // sum of column value
        labelMap[pixelColor][2]+=originalArr[i][j]; // area
      }
    }
  }
}

void findLabelInfo(unordered_map<int, vector<double>> &labelMap) {
  for (auto pair : labelMap) {
    int labelNumber = pair.first;
    double rowSum = labelMap[labelNumber][0];
    double colSum = labelMap[labelNumber][1];
    double area = labelMap[labelNumber][2];
    double centerRow = rowSum/area;
    double centerCol = colSum/area;
    labelMap[labelNumber][3] = centerRow; // row center
    labelMap[labelNumber][4] = centerCol; // col center
  }
}

vector<vector<int>> ComputeProperties(vector<vector<int>> &array2D, vector<vector<int>> &originalArr) {
  vector<vector<int>> outputs;
// number of rows and columns
  size_t rows = array2D.size();
  size_t cols = array2D[0].size();

  // Counter starts at 1 so we can use 0 to check if we have seen the label
  unordered_map<int, vector<double>> labelMap = {};

  // Assembles sums and other info through scanning through each pixel of image
  populateSumsAndArea(array2D, labelMap, originalArr);

  // Organize additional computation results  into vectors for easy access
  findLabelInfo(labelMap);

  for (auto pair : labelMap) {
    int labelNumber = pair.first;
    vector<int> coordinates;
    vector<double> labelInfo = pair.second;

    double area = labelInfo[2];
    double x = labelInfo[3];
    double y = labelInfo[4];
    coordinates.push_back(x);
    coordinates.push_back(y);
    outputs.push_back(coordinates);
  }
  return outputs;
}

void drawLineForArrCoord(int i, std::vector<std::vector<int>> &array2D, int j, size_t numRows, size_t numCols, ComputerVisionProjects::Image &image)
{
  vector<double> points;
  double theta = i * M_PI / array2D.size();
  double rho = j * sqrt(numRows * numRows + numCols * numCols) / array2D[i].size();

  double x = 0;
  double y = (rho) / sin(theta);

  if (y >= 0 && y < numCols)
  {
    points.push_back(x);
    points.push_back(y);
  }

  y = 0;
  x = (rho) / cos(theta);

  if (x >= 0 && x < numRows)
  {
    points.push_back(x);
    points.push_back(y);
  }

  x = numRows - 1;
  y = (rho - (x * cos(theta))) / sin(theta);

  if (y >= 0 && y < numCols)
  {
    points.push_back(x);
    points.push_back(y);
  }

  y = numCols - 1;
  x = (rho - (y * sin(theta))) / cos(theta);

  if (x >= 0 && x < numRows)
  {
    points.push_back(x);
    points.push_back(y);
  }

  // cout << rho << " " << theta << endl;
  // cout << int(points[0])<< "," << int(points[1]) << " " << int(points[2]) << "," << int(points[3]) << endl;
  // cout << endl;
  DrawLine(int(points[0]), int(points[1]), int(points[2]), int(points[3]), 255, &image);
}
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

  vector<vector<int>> originalArr = array2D;
  
  // Make the image binary
  for (int i = 0; i < array2D.size(); i++) {
    for (int j = 0; j < array2D[i].size(); j++) {
      if (array2D[i][j] <= threshold)
        array2D[i][j] = 0;
    }
  }

  PerformSequentialLabeling(array2D);

  vector<vector<int>> points = ComputeProperties(array2D, originalArr);
  for (vector<int> point: points) {
    drawLineForArrCoord(point[0], array2D, point[1], numRows, numCols, image);
  }

  if (!WriteImage(output_gray_level_line_filename, image)){
    cout << "Can't write to file " << output_gray_level_line_filename << endl;
  }
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
