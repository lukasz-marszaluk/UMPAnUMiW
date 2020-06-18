#ifndef _DOCUMENT_
#define _DOCUMENT_

#include "image.hpp"
#include <vector>

class detected_object
{
private:
  char *data;
  int width;
  int height;
  
public:
  detected_object(int _width, int _height); // todo
  ~detected_object(); // todo

  void toggle_cell (int x, int y); // todo
};

struct point 
{
  int x;
  int y;
};
  
class document
{
private:
  image *img;
  point doc_corners [4];
  point shift [4];

public:
  void recognize_document ();
  void stretch_document (); // todo
  void improve_colors (); // todo

  image *get_document_image ();

  document(image *_img);
  ~document();

private:
  std::vector<detected_object*> separate_objects_on_image(grayscale_image *image);
  detected_object* find_biggest_object (std::vector<detected_object*> objects); // todo
  void object_to_interior_contour (detected_object *object); // todo
  void extract_document_corners (detected_object *contours); // todo

  void separate_object_on_image_recursive(detected_object *object, grayscale_image *image, int x, int y);
};

#endif //_DOCUMENT_