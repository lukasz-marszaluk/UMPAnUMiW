#ifndef _DOCUMENT_
#define _DOCUMENT_

#include "image.hpp"
#include <vector>

class detected_object
{
public:
  int width;
  int height;

private:
  unsigned char *data;

public:
  detected_object(int _width, int _height);
  ~detected_object();

  void toggle_cell(int x, int y);
  unsigned char read_cell(int x, int y);
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
  detected_object *extract_document_outline(grayscale_image *image);
};

#endif //_DOCUMENT_