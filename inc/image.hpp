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
    image(std::string filename);
    image();
    ~image();

    unsigned char get_pixel(int x, int y, int c);
    unsigned char get_pixel(int x, int y);
    unsigned char get_pixel(int index);

    void set_pixel(int x, int y, int c, unsigned char value);
    void set_pixel(int x, int y, unsigned char r, unsigned char b, unsigned char g);

    void save (std::string filename);
    void draw_text ();
};

#endif // _IMAGE_