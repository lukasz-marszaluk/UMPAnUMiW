#ifndef _IMAGE_
#define _IMAGE_

#include <string>

enum result
{
    OK,
    STBI_ERROR,
    MALLOC_FAULT,
    UNDEFINED,
    UNSUPPORTED_FORMAT
};

class grayscale_image;

class image
{
public:
    result last_result;
    int width;
    int height;
private:
    unsigned char *data;

public:
    image(int _width, int _height);
    image(std::string &filename);
    image(image *img);
    image(grayscale_image *img);
    image();
    ~image();

    unsigned char get_pixel(int x, int y, int c);
    unsigned char get_pixel(int x, int y);
    unsigned char get_pixel(int index);

    void set_pixel(int x, int y, int c, unsigned char value);
    void set_pixel(int x, int y, unsigned char r, unsigned char b, unsigned char g);

    void save (std::string filename);

    void threshold_image (unsigned char threshold); // TODO
    int* get_histogram (); // TODO
    void apply_lookup_tables (unsigned char *red_lut, unsigned char *green_lut, unsigned char *blue_lut); // TODO
};

class grayscale_image
{
public:
    result last_result;
    int width;
    int height;
private:
    unsigned char *data;

public:
    grayscale_image(image *img);
    grayscale_image(grayscale_image *img);
    ~grayscale_image();

    unsigned char get_pixel(int x, int y);
    unsigned char get_pixel(int index);
    void set_pixel(int x, int y, unsigned char value);

    void gaussian_blur ();
    void canny_edge_detection ();
};

#endif // _IMAGE_