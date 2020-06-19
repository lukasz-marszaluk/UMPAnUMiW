#include "../inc/document.hpp"
#include <cstring>

document::document(image *_img)
{
  img = new image(_img);
}

document::~document()
{
  if (img != NULL)
    delete img;
}

void document::recognize_document()
{
  grayscale_image *bw_img;
  detected_object *document_outline;

  bw_img = new grayscale_image(img);
  bw_img->canny_edge_detection();

  document_outline = extract_document_outline(bw_img);

  delete document_outline;
  delete bw_img;
}

image *document::get_document_image()
{
  return img;
}

detected_object *document::extract_document_outline(grayscale_image *image)
{
  detected_object *result = new detected_object(image->width, image->height);

  int xi, yi;

  //from top
  for (xi = 0; xi < image->width; xi++)
  {
    for (yi = 0; yi < image->height; yi++)
    {
      if (image->get_pixel(xi, yi) == 255)
      {
        result->toggle_cell(xi, yi);
        break;
      }
    }
  }

  //from bottom
  for (xi = 0; xi < image->width; xi++)
  {
    for (yi = image->height - 1; yi >= 0; yi--)
    {
      if (image->get_pixel(xi, yi) == 255)
      {
        result->toggle_cell(xi, yi);
        break;
      }
    }
  }

  //from left
  for (yi = 0; yi < image->height; yi++)
  {
    for (xi = 0; xi < image->width; xi++)
    {
      if (image->get_pixel(xi, yi) == 255)
      {
        result->toggle_cell(xi, yi);
        break;
      }
    }
  }

  //from right
  for (yi = 0; yi < image->height; yi++)
  {
    for (xi = image->width - 1; xi >= 0; xi--)
    {
      if (image->get_pixel(xi, yi) == 255)
      {
        result->toggle_cell(xi, yi);
        break;
      }
    }
  }

  return result;
}

detected_object::detected_object(int _width, int _height)
{
  width = _width;
  height = _height;
  data = (unsigned char *)malloc(width * height);

  memset(data, 0, width * height);
}

detected_object::~detected_object()
{
  free(data);
}

void detected_object::toggle_cell(int x, int y)
{
  data[y * width + x] ^= 255;
}

unsigned char detected_object::read_cell(int x, int y)
{
  return data[y * width + x];
}
