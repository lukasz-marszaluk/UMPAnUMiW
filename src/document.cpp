#include "../inc/document.hpp"

document::document(image *_img)
{
  img = new image (_img);
}

document::~document()
{
  if (img != NULL)
    delete img;
}

void document::recognize_document ()
{
  grayscale_image *bw_img;
  std::vector<detected_object*> detected_objects;
  detected_object *document_outline;

  bw_img = new grayscale_image(img);
  bw_img->canny_edge_detection();

  detected_objects = separate_objects_on_image (bw_img);
  document_outline = find_biggest_object (detected_objects);

  object_to_interior_contour (document_outline);
  extract_document_corners (document_outline);

  delete bw_img;

  for (detected_object *obj : detected_objects)
    delete obj;

  delete document_outline;
}

image *document::get_document_image()
{
  return img;
}

std::vector<detected_object *> document::separate_objects_on_image(grayscale_image *image)
{
  int xi, yi;
  std::vector <detected_object *> objects;

  for (yi = 0; yi < image->height; yi++)
  {
    for (xi = 0; xi < image->width; xi++)
    {
      if (image->get_pixel(xi, yi))
      {
        
      }

    }
  }
  
  return objects;
}

detected_object* document::find_biggest_object (std::vector<detected_object*> objects)
{

}

void document::object_to_interior_contour (detected_object *object)
{

}

void document::extract_document_corners (detected_object *contours)
{

}

void document::separate_object_on_image_recursive(detected_object *object, grayscale_image *image, int x, int y)
{
  if (x < 0 || x >= image->width)
    return;
  if (y < 0 || y >= image->height)
    return;

  if (image->get_pixel(x, y) == 0)
    return;

  image->set_pixel (x, y, 0);
  // set object value

  separate_object_on_image_recursive (object, image, x - 1, y - 1);
  separate_object_on_image_recursive (object, image, x, y - 1);
  separate_object_on_image_recursive (object, image, x + 1, y - 1);
  separate_object_on_image_recursive (object, image, x - 1, y);
  separate_object_on_image_recursive (object, image, x + 1, y);
  separate_object_on_image_recursive (object, image, x - 1, y + 1);
  separate_object_on_image_recursive (object, image, x, y + 1);
  separate_object_on_image_recursive (object, image, x + 1, y + 1);
}