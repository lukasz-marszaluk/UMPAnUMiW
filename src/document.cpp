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

std::vector<detected_object*> document::separate_objects_on_image(grayscale_image*)
{

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