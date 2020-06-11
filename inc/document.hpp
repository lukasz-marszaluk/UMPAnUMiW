#ifndef _DOCUMENT_
#define _DOCUMENT_

#include "image.hpp"
#include <vector>

class detected_object
{
};

struct point 
{
  int x;
  int y;
};

class document
{
private:
  image img;
  point doc_corners [4];

public:
  void recognize_document ();
  void stretch_document ();
  void improve_colors ();
  image get_document_image ();

  document(image img);
  ~document();

private:
  std::vector<detected_object> separate_objects_on_image();
  detected_object find_biggest_object (std::vector<detected_object> objects);
  void object_to_interior_contour (detected_object *object);
  void extract_document_corners (detected_object contours);
};

#endif //_DOCUMENT_