#include "../inc/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define NDEBUG // comment that line to enable asserts

#include "../third_party/stb_image.h"
#include "../third_party/stb_image_write.h"

#include <iostream>
#include <cassert>
#include <cmath>

image::image(int _width, int _height)
{
    last_result = UNDEFINED;
    width = _width;
    height = _height;

    data = NULL;
    data = (unsigned char *)stbi__malloc(width * height * 3);

    if (data != NULL)
        last_result = OK;
}

image::image(std::string &filename)
{
    int comp; // not used

    last_result = UNDEFINED;

    data = NULL;
    data = stbi_load(filename.c_str(), &width, &height, &comp, 3);

    if (data != NULL)
        last_result = OK;
    else
        printf("Cannot open file: %s\nReason: %s\n", filename, stbi_failure_reason());
}

image::image(image *img)
{
    last_result = result::UNDEFINED;

    if (img->last_result != result::OK)
        return;

    width = img->width;
    height = img->height;

    data = NULL;
    data = (unsigned char *)stbi__malloc(width * height * 3);

    if (data != NULL)
        last_result = OK;
    else
        return;

    memcpy(data, img->data, width * height * 3);
    last_result = img->last_result;
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

    data = NULL;
}

unsigned char image::get_pixel(int x, int y, int c)
{
    //assert((x >= 0) && (x < width));
    //assert((y >= 0) && (y < height));
    //assert((c >= 0) && (c < 3));

    return data[(y * width + x) * 3 + c];
}

unsigned char image::get_pixel(int x, int y)
{
    //assert((x >= 0) && (x < width));
    //assert((y >= 0) && (y < height));

    return (unsigned char)(((int)data[(y * width + x) * 3 + 0] +
                            (int)data[(y * width + x) * 3 + 1] +
                            (int)data[(y * width + x) * 3 + 2]) /
                           3);
}

unsigned char image::get_pixel(int index)
{
    //assert((index >= 0) && (index < (width * height * 3)));

    return data[index];
}

void image::set_pixel(int x, int y, int c, unsigned char value)
{
    //assert((x >= 0) && (x < width));
    //assert((y >= 0) && (y < height));
    //assert((c >= 0) && (c < 3));

    data[(y * width + x) * 3 + c] = value;
}

void image::set_pixel(int x, int y, unsigned char r, unsigned char b, unsigned char g)
{
    //assert((x >= 0) && (x < width));
    //assert((y >= 0) && (y < height));

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

int *image::get_histogram()
{
    int yi, xi;
    int *histogram = new int[256];

    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    for (yi = 0; yi < height; yi++)
        for (xi = 0; xi < width; xi++)
            histogram[get_pixel(xi, yi)] += 1;

    return histogram;
}

void image::canny_edge_detection()
{
    int xi, yi, xii, yii;
    unsigned char *grayscale_img = new unsigned char[width * height];

    // remove noise from image
    gaussian_blur();

    // convert to grayscale
    for (yi = 0; yi < height; yi++)
        for (xi = 0; xi < width; xi++)
            grayscale_img[yi * width + xi] = get_pixel(xi, yi);

    // calculate gradient
    int grad_x, grad_y;
    int *gradient_value = new int[width * height];
    double *gradient_direction = new double[width * height];

    memset(gradient_value, 0, sizeof(int));
    memset(gradient_direction, 0, sizeof(double));

    int convolution_mask_x[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 2};

    int convolution_mask_y[] = {
        -1, -2, -1,
        0, 0, 0,
        1, 2, 1};

    for (yi = 1; yi < height - 1; yi++)
    {
        for (xi = 1; xi < width - 1; xi++)
        {
            grad_x = 0;
            grad_y = 0;

            for (yii = 0; yii < 3; yii++)
            {
                for (xii = 0; xii < 3; xii++)
                {
                    grad_y +=
                        grayscale_img[(yi + yii - 1) * width + (xi + xii - 1)] *
                        convolution_mask_y[yii * width + xi];

                    grad_x +=
                        grayscale_img[(yi + yii - 1) * width + (xi + xii - 1)] *
                        convolution_mask_x[yii * width + xi];
                }
            }

            gradient_value[yi * width + xi] = sqrt(grad_x * grad_x + grad_y * grad_y);
            gradient_direction[yi * width + xi] = atan((double)grad_y / grad_x);
        }
    }

    for (yi = 0; yi < height; yi++)
    {
        for (xi = 0; xi < width; xi++)
        {
            set_pixel(xi, yi, 0, (unsigned char)(gradient_value[yi * width + xi] / 100));
            set_pixel(xi, yi, 1, (unsigned char)(gradient_value[yi * width + xi] / 100));
            set_pixel(xi, yi, 2, (unsigned char)(gradient_value[yi * width + xi] / 100));
        }
    }

    delete[] gradient_direction;
    delete[] gradient_value;
    delete[] grayscale_img;
}

void image::gaussian_blur()
{
    int xi, yi, ci, xii, yii;
    double pixel;
    image *temp_img = new image(this);

    double gaussian_kernel[5] = {0.117647059, 0.235294118, 0.294117647, 0.235294118, 0.117647059};

    // apply filter in horizontal direction
    for (yi = 0; yi < height; yi++)
    {
        for (xi = 2; xi < width - 2; xi++)
        {
            for (ci = 0; ci < 3; ci++)
            {
                pixel = 0.0;

                for (xii = 0; xii < 5; xii++)
                    pixel += gaussian_kernel[xii] * get_pixel(xi + xii - 2, yi, ci);

                temp_img->set_pixel(xi, yi, ci, (unsigned char)pixel);
            }
        }
    }

    // apply filter in horizontal direction
    for (yi = 2; yi < height - 2; yi++)
    {
        for (xi = 0; xi < width; xi++)
        {
            for (ci = 0; ci < 3; ci++)
            {
                pixel = 0.0;

                for (yii = 0; yii < 5; yii++)
                    pixel += gaussian_kernel[yii] * temp_img->get_pixel(xi, yi + yii - 2, ci);

                set_pixel(xi, yi, ci, (unsigned char)pixel);
            }
        }
    }

    delete temp_img;
}
