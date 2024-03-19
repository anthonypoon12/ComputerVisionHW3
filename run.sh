#!/bin/bash

make all
./h1 InputImagesHomework3/hough_simple_1.pgm 1output.pgm
./h2 1output.pgm 40 1output2.pgm
./h3 1output2.pgm 1output3.pgm a.txt
./h4 InputImagesHomework3/hough_simple_1.pgm a.txt 170 1output4.pgm


./h1 InputImagesHomework3/hough_simple_2.pgm 2output.pgm
./h2 2output.pgm 40 2output2.pgm
./h3 2output2.pgm 2output3.pgm b.txt
./h4 InputImagesHomework3/hough_simple_2.pgm b.txt 130 2output4.pgm

./h1 InputImagesHomework3/hough_complex_1.pgm 3output.pgm
./h2 3output.pgm 30 3output2.pgm
./h3 3output2.pgm 3output3.pgm c.txt
./h4 InputImagesHomework3/hough_complex_1.pgm c.txt 70 3output4.pgm

make clean