#ifndef _IO_
#define _IO_

#include <string>

struct params
{
  std::string output_file;
  std::string input_file;

  bool read_photo_from_file;
  bool disable_cutting_out_document;
  bool disable_color_correction;
  bool save_preview;
  bool print_help;
  bool error;
};

params read_arguments (int argc, char **argv);

void print_help (char * argv_0);

void print_message (std::string msg);

#endif //_IO_
