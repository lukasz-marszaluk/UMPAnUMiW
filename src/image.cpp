#include "../inc/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "../third_party/stb_image.h"
#include "../third_party/stb_image_write.h"

#include <iostream>
#include <cassert>

image::image(int _width, int _height)
{
    last_result = UNDEFINED;
    width = _width;
    height = _height;
    data = (unsigned char *)stbi__malloc(width * height * 3);

    if (data != NULL)
        last_result = OK;
}

image::image(std::string filename)
{
    int comp; // not used

    last_result = UNDEFINED;
    data = stbi_load(filename.c_str(), &width, &height, &comp, 3);

    if (data != NULL)
        last_result = OK;
    else
        printf(
            "Cannot open file: %s\nReason: %s\n",
            filename,
            stbi_failure_reason());
}

image::image()
{
    last_result = UNDEFINED;
    data = NULL;
}

image::~image()
{
    if (data != NULL)
        stbi_image_free(data);
}

unsigned char image::get_pixel(int x, int y, int c)
{
    assert((x >= 0) && (x < width));
    assert((y >= 0) && (y < height));
    assert((c >= 0) && (c < 3));

    return data[(y * width + x) * 3 + c];
}

unsigned char image::get_pixel(int x, int y)
{
    assert((x >= 0) && (x < width));
    assert((y >= 0) && (y < height));

    return (unsigned char)(((int)data[(y * width + x) * 3 + 0] +
                            (int)data[(y * width + x) * 3 + 1] +
                            (int)data[(y * width + x) * 3 + 2]) /
                           3);
}

unsigned char image::get_pixel(int index)
{
    assert((index >= 0) && (index < (width * height * 3)));

    return data[index];
}

void image::set_pixel(int x, int y, int c, unsigned char value)
{
    assert((x >= 0) && (x < width));
    assert((y >= 0) && (y < height));
    assert((c >= 0) && (c < 3));

    data[(y * width + x) * 3 + c] = value;
}

void image::set_pixel(int x, int y, unsigned char r, unsigned char b, unsigned char g)
{
    assert((x >= 0) && (x < width));
    assert((y >= 0) && (y < height));

    data[(y * width + x) * 3 + 0] = r;
    data[(y * width + x) * 3 + 1] = g;
    data[(y * width + x) * 3 + 2] = b;
}

void image::save(std::string filename)
{
    int result;
    std::string ext;

    if (last_result != OK)
        return;

    ext = filename.substr(filename.find_last_of('.', filename.length()));

    if (ext.compare(".png"))
        result = stbi_write_png(filename.c_str(), width, height, 3, data, 0);
    else if (ext.compare(".bmp"))
        result = stbi_write_bmp(filename.c_str(), width, height, 3, data);
    else if (ext.compare(".jpg"))
        result = stbi_write_jpg(filename.c_str(), width, height, 3, data, 95);
    else if (ext.compare(".tga"))
        result = stbi_write_tga(filename.c_str(), width, height, 3, data);
    else
        last_result = UNSUPPORTED_FORMAT;

    if (result == 0)
        last_result = STBI_ERROR;
}
