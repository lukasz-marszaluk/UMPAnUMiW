#ifndef _DOCUMENT_
#define _DOCUMENT_

#include "image.hpp"
#include "common.hpp"
#include <vector>

class detected_object
{
private:
  std::vector<point> data;

public:
  detected_object();
  ~detected_object();

  void add_cell(int x, int y);
  bool read_cell(int x, int y);
};

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
  int find_biggest_object(int *separated_image);
};

#endif //_DOCUMENT_