Anthony Poon
Computational Vision Homework 3

1. All four parts of the assignment have been completed.
2. Have some problems when set very low thresholds in part 1. 
3. All programs runs as directed in assignment details.
4. Input files same as the ones provided.

Program_1 Edge image
    - Filter used is: Sobel 3x3 operator
    - applies filter to image, uses zero padding (treats pixels that are out of bounds as zero)
    How to Use: ./h1 input_image_filename output_image_filename

Program_2 Edge thresholding.
    - The threshold used is: 40
    - Converts the image to a binary image of black and white
    - Similar code to previous assignment
    How to use: ./h2 input_edge_image_filename threshold output_image_filename

Program_3 Computation of the Hough image
    - Creates a new image where each edge pixel is converted to a line
    - the theta value is the row coordinate and the rho value is the column coordinate
    - creates a text file of these values
    - dimensions can be controlled with resolution values in program
    How to use: ./h3 input_binary_image_filename output_hough_image_filename output_text_filename
    
Program_4 Line detection
    - The threshold used is: 150
    - Eliminates values that are below threshold
    - Uses sequential labeling (similar to previous assignment) to find the central edge
    - Uses weighted averages with the original array values to find the relevant edge pixel
    - Draws line on original image to indicate edge value (Uses white to be clearly visible)
    How to use: ./h4  input_image_filename input_text_filename threshold output_image_filename