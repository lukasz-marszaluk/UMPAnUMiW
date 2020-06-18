# UMPAnUMiW
Simple documment scanner software.

## Authors:
* Marszaluk Lukasz
* Wolny Dominik

## Scanned document requirements
1. Visible margin
2. Coherent, contrasting with document background
3. Document not touching edges of image (not cropped in any way)  

## Tasks
1. project structure
2. basic image i/o and manipulation library
3. read program parameters
4. detecting corners of document inside image
5. stretching corners to rectangle
6. analyze and correct document colors with look-up tables

### Parameters
-a4h - stretch to format of horizontal a4 page
-a4p - stretch to format of portrait a4 page
-tint - disable removing tint
-contrast - disable increasing contrast
-o [filename] - output filename with extension
-i [filename] - ommit taking photo, load this file
-h --help
-d - debug (save all data created during proccessing e.g. histograms, image with drawed corners, stretch map ect.)
