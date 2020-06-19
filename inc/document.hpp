#ifndef _DOCUMENT_
#define _DOCUMENT_

#include "image.hpp"
#include "common.hpp"


class document
{
private:
  image *img;
  point doc_corners[4];
  point shift[4];

public:
  void recognize_document();
  void stretch_document(); // todo
  void improve_colors();   // todo

  image *get_document_image();

  document(image *_img);
  ~document();

private:
  void separate_objects_from_image(grayscale_image *image, int *separated_image);
  void take_out_object (grayscale_image *image, int *separated_image, int object_index, int x, int y);
  int find_biggest_object(int *separated_image);
};

#endif //_DOCUMENT_