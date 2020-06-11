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
    image(image * img);
    image();
    ~image();

    unsigned char get_pixel(int x, int y, int c);
    unsigned char get_pixel(int x, int y);
    unsigned char get_pixel(int index);

    void set_pixel(int x, int y, int c, unsigned char value);
    void set_pixel(int x, int y, unsigned char r, unsigned char b, unsigned char g);

    void save (std::string filename);

    void canny_edge_detection ();
    void gaussian_blur ();
    void threshold_image (unsigned char threshold);
    void double_threshold_image (unsigned char lower_threshold, unsigned char higher_threshold);
    int* get_histogram ();
    void apply_lookup_tables (unsigned char *red_lut, unsigned char *green_lut, unsigned char *blue_lut);
};

#endif // _IMAGE_