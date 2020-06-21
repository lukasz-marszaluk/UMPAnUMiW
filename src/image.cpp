#include "../inc/image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#define NDEBUG // comment that line to enable asserts

#include "../inc/common.hpp"

#include "../third_party/stb_image.h"
#include "../third_party/stb_image_write.h"

#include <iostream>
#include <cassert>

#define _USE_MATH_DEFINES
#include <math.h>

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

image::image(grayscale_image *img)
{
    int yi, xi;
    unsigned char pixel;

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

    for (yi = 0; yi < height; yi++)
    {
        for (xi = 0; xi < width; xi++)
        {
            pixel = img->get_pixel(xi, yi);
            set_pixel(xi, yi, pixel, pixel, pixel);
        }
    }

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
    return data[(y * width + x) * 3 + c];
}

unsigned char image::get_pixel(int x, int y)
{
    return (unsigned char)(((int)data[(y * width + x) * 3 + 0] +
                            (int)data[(y * width + x) * 3 + 1] +
                            (int)data[(y * width + x) * 3 + 2]) /
                           3);
}

unsigned char image::get_pixel(int index)
{
    return data[index];
}

void image::set_pixel(int index, unsigned char value)
{
    data[index] = value;
}

void image::set_pixel(int x, int y, int c, unsigned char value)
{
    data[(y * width + x) * 3 + c] = value;
}

void image::set_pixel(int x, int y, unsigned char r, unsigned char b, unsigned char g)
{
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

    if (ext.compare(".png") == 0)
        result = stbi_write_png(filename.c_str(), width, height, 3, data, 0);
    else if (ext.compare(".bmp") == 0)
        result = stbi_write_bmp(filename.c_str(), width, height, 3, data);
    else if (ext.compare(".jpg") == 0)
        result = stbi_write_jpg(filename.c_str(), width, height, 3, data, 95);
    else if (ext.compare(".tga") == 0)
        result = stbi_write_tga(filename.c_str(), width, height, 3, data);
    else
        last_result = UNSUPPORTED_FORMAT;

    if (result == 0)
        last_result = STBI_ERROR;
}

int *image::get_histogram(int channel)
{
    int yi, xi;
    int *histogram = new int[256];

    for (int i = 0; i < 256; i++)
        histogram[i] = 0;

    for (yi = 0; yi < height; yi++)
        for (xi = 0; xi < width; xi++)
            histogram[get_pixel(xi, yi, channel)] += 1;

    return histogram;
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

grayscale_image::grayscale_image(image *img)
{
    last_result = UNDEFINED;

    width = img->width;
    height = img->height;

    data = NULL;
    data = (unsigned char *)stbi__malloc(width * height);

    if (data != NULL)
        last_result = OK;

    int yi, xi;

    for (yi = 0; yi < height; yi++)
        for (xi = 0; xi < width; xi++)
            data[yi * width + xi] = img->get_pixel(xi, yi);
}

grayscale_image::grayscale_image(grayscale_image *img)
{
    last_result = UNDEFINED;

    if (img->last_result != OK)
        return;

    width = img->width;
    height = img->height;

    data = NULL;
    data = (unsigned char *)stbi__malloc(width * height);

    if (data != NULL)
        last_result = OK;
    else
        return;

    memcpy(data, img->data, width * height);
    last_result = OK;
}

grayscale_image::~grayscale_image()
{
    if (data != NULL)
        stbi_image_free(data);

    data = NULL;
}

unsigned char grayscale_image::get_pixel(int x, int y)
{
    return data[y * width + x];
}

unsigned char grayscale_image::get_pixel(int index)
{
    return data[index];
}

void grayscale_image::set_pixel(int x, int y, unsigned char value)
{
    data[y * width + x] = value;
}

void grayscale_image::gaussian_blur()
{
    int xi, yi, xii, yii;
    double pixel;
    grayscale_image *temp_img = new grayscale_image(this);

    double gaussian_kernel[5] = {0.117647059, 0.235294118, 0.294117647, 0.235294118, 0.117647059};

    // apply filter in horizontal direction
    for (yi = 0; yi < height; yi++)
    {
        for (xi = 2; xi < width - 2; xi++)
        {
            pixel = 0.0;

            for (xii = 0; xii < 5; xii++)
                pixel += gaussian_kernel[xii] * get_pixel(xi + xii - 2, yi);

            temp_img->set_pixel(xi, yi, (unsigned char)pixel);
        }
    }

    // apply filter in horizontal direction
    for (yi = 2; yi < height - 2; yi++)
    {
        for (xi = 0; xi < width; xi++)
        {
            pixel = 0.0;

            for (yii = 0; yii < 5; yii++)
                pixel += gaussian_kernel[yii] * temp_img->get_pixel(xi, yi + yii - 2);

            set_pixel(xi, yi, (unsigned char)pixel);
        }
    }

    delete temp_img;
}

void grayscale_image::canny_edge_detection()
{
    int xi, yi, xii, yii, i;

    // remove noise from image
    gaussian_blur();

    // calculate gradient
    int grad_x, grad_y;
    int *gradient_value = new int[width * height];
    int *gradient_direction = new int[width * height];
    int *non_max_suppressed = new int[width * height];
    int max_grad_value = 0;
    int pixel;

    memset(gradient_value, 0, sizeof(int) * width * height);
    memset(gradient_direction, 0, sizeof(int) * width * height);
    memset(non_max_suppressed, 0, sizeof(int) * width * height);

    int convolution_mask_x[] = {
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1};

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
                    grad_x += convolution_mask_x[yii * 3 + xii] *
                              get_pixel(xi + xii - 1, yi + yii - 1);

                    grad_y += convolution_mask_y[yii * 3 + xii] *
                              get_pixel(xi + xii - 1, yi + yii - 1);
                }
            }

            gradient_value[yi * width + xi] = sqrt(grad_x * grad_x + grad_y * grad_y);
            gradient_direction[yi * width + xi] = round_angle_45(atan2(grad_y, grad_x) * 180.0 / M_PI);

            if (max_grad_value < gradient_value[yi * width + xi])
                max_grad_value = gradient_value[yi * width + xi];
        }
    }

    for (i = 0; i < width * height; i++)
        gradient_value[i] = gradient_value[i] * 255 / max_grad_value;
    

    // non_max suppression
    for (yi = 1; yi < height - 1; yi++)
    {
        for (xi = 1; xi < width - 1; xi++)
        {
            switch (gradient_direction[yi * width + xi])
            {
            case 0:
                if (gradient_value[yi * width + xi] < gradient_value[yi * width + xi + 1] || gradient_value[yi * width + xi] < gradient_value[yi * width + xi - 1])
                    non_max_suppressed[yi * width + xi] = 0;
                else
                    non_max_suppressed[yi * width + xi] = gradient_value[yi * width + xi];
                break;
            case 45:
                if (gradient_value[yi * width + xi] < gradient_value[(yi + 1) * width + xi - 1] || gradient_value[yi * width + xi] < gradient_value[(yi - 1) * width + xi + 1])
                    non_max_suppressed[yi * width + xi] = 0;
                else
                    non_max_suppressed[yi * width + xi] = gradient_value[yi * width + xi];
                break;
            case 90:
                if (gradient_value[yi * width + xi] < gradient_value[(yi + 1) * width + xi] || gradient_value[yi * width + xi] < gradient_value[(yi - 1) * width + xi])
                    non_max_suppressed[yi * width + xi] = 0;
                else
                    non_max_suppressed[yi * width + xi] = gradient_value[yi * width + xi];
                break;
            case 135:
                if (gradient_value[yi * width + xi] < gradient_value[(yi - 1) * width + xi + -1] || gradient_value[yi * width + xi] < gradient_value[(yi + 1) * width + xi + 1])
                    non_max_suppressed[yi * width + xi] = 0;
                else
                    non_max_suppressed[yi * width + xi] = gradient_value[yi * width + xi];
                break;
            }
        }
    }

    memcpy(gradient_value, non_max_suppressed, width * height * sizeof(int));

    // double threshold
    int higher_threshold = max_grad_value / 10;
    int lower_threshold = higher_threshold / 20;

    for (i = 0; i < width * height; i++)
    {
        if (gradient_value[i] > higher_threshold)
            gradient_value[i] = 255;
        else if (gradient_value[i] < lower_threshold)
            gradient_value[i] = 0;
        else
            gradient_value[i] = 127;
    }

    memset(data, 0, width * height);

    // hysteresis with output save
    for (yi = 1; yi < height - 1; yi++)
    {
        for (xi = 1; xi < width - 1; xi++)
        {
            if (gradient_value[yi * width + xi] == 255)
                set_pixel(xi, yi, 255);
            else if (gradient_value[yi * width + xi] == 0)
                set_pixel(xi, yi, 0);
            else if (
                (gradient_value[(yi - 1) * width + (xi - 1)] == 255) ||
                (gradient_value[(yi - 1) * width + (xi + 0)] == 255) ||
                (gradient_value[(yi - 1) * width + (xi + 1)] == 255) ||
                (gradient_value[(yi + 0) * width + (xi - 1)] == 255) ||
                (gradient_value[(yi + 0) * width + (xi + 1)] == 255) ||
                (gradient_value[(yi + 1) * width + (xi - 1)] == 255) ||
                (gradient_value[(yi + 1) * width + (xi + 0)] == 255) ||
                (gradient_value[(yi + 1) * width + (xi + 1)] == 255))
                set_pixel(xi, yi, 255);
            else
                set_pixel(xi, yi, 0);
        }
    }

    delete[] gradient_direction;
    delete[] gradient_value;
    delete[] non_max_suppressed;
}

void image::unified_transform(point *transform_matrix, int martix_size)
{
    int xi, yi, ci;
    int new_width, new_height;
    int temp;

    if (martix_size != 4)
        return;

    // find new dimensions of output image
    new_width = transform_matrix[1].x - transform_matrix[0].x;
    temp = transform_matrix[3].x - transform_matrix[2].x;

    if (new_width > temp)
        new_width = temp;

    new_height = transform_matrix[2].y - transform_matrix[0].y;
    temp = transform_matrix[3].y - transform_matrix[1].y;

    if (new_height > temp)
        new_height = temp;

    image *output = new image(new_width, new_height);
    point_double shift;

    // proper transformation
    for (yi = 0; yi < output->height; yi++)
    {
        for (xi = 0; xi < output->width; xi++)
        {
            shift = output->get_shift(xi, yi, transform_matrix);
            output->transform_pixel (this, shift, xi, yi);

            //for (ci = 0; ci < 3; ci++)
            //   output->set_pixel(xi, yi, ci, get_pixel((int)shift.x, (int)shift.y, ci));
        }
    }

    // save output to "this*"
    width = output->width;
    height = output->height;

    stbi_image_free(data);
    data = (unsigned char *)stbi__malloc(width * height * 3);
    memcpy(data, output->data, width * height * 3);

    delete output;
}

point_double image::get_shift(int x, int y, point *shift)
{
    double shift_on_top, shift_on_bottom;
    point_double result;

    // horizontal
    shift_on_top = (double)(shift[0].x * (width - x) + shift[1].x * x) / width;
    shift_on_bottom = (double)(shift[2].x * (width - x) + shift[3].x * x) / width;
    result.x = (shift_on_top * (height - y) + shift_on_bottom * y) / height;

    // vertical
    shift_on_top = (double)(shift[0].y * (height - y) + shift[2].y * y) / height;
    shift_on_bottom = (double)(shift[1].y * (height - y) + shift[3].y * y) / height;
    result.y = (shift_on_top * (width - x) + shift_on_bottom * x) / width;

    return result;
}

void image::transform_pixel (image *src, point_double shift, int dest_x, int dest_y)
{
    double fraction_x, fraction_y;
    double pixel;
    double weight [4];
    int index [4];
    int ci, pi;

    fraction_x = shift.x - (int)shift.x;
    fraction_y = shift.y - (int)shift.y;

    weight [0] = (1.0 - fraction_x) * (1.0 - fraction_y);
    weight [1] = fraction_x * (1.0 - fraction_y);
    weight [2] = (1.0 - fraction_x) * fraction_y;
    weight [3] = fraction_x * fraction_y;

    index [0] = ((int)shift.y * src->width + (int)shift.x) * 3;
    index [1] = index [0] + 1 * 3;
    index [2] = index [0] + src->width * 3;
    index [3] = index [2] + 1 * 3;

    for (ci = 0; ci < 3; ci++)
    {
        pixel = 0.0;
        
        for (pi = 0; pi < 4; pi++)
            pixel += weight [pi] * src->get_pixel(index [pi] + ci);

        set_pixel(dest_x, dest_y, ci, (unsigned char)pixel);
    }    
}

void image::apply_lookup_tables(unsigned char *red_lut, unsigned char *green_lut, unsigned char *blue_lut)
{
    int xi, yi;

    for (yi = 0; yi < height; yi++)
    {
        for (xi = 0; xi < width; xi++)
        {
            set_pixel(xi, yi, 0, red_lut[get_pixel(xi, yi, 0)]);
            set_pixel(xi, yi, 1, green_lut[get_pixel(xi, yi, 1)]);
            set_pixel(xi, yi, 2, blue_lut[get_pixel(xi, yi, 2)]);
        }
    }
}

void image::remove_tint()
{
    int i;

    int *hist_red = get_histogram(0);
    int *hist_green = get_histogram(1);
    int *hist_blue = get_histogram(2);

    int difference_red_green = 0;
    int difference_red_blue = 0;





    delete[] hist_red;
    delete[] hist_green;
    delete[] hist_blue;
}