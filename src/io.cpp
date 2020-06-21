#include "../inc/io.hpp"

#include <cstring>
#include <iostream>

params read_arguments(int argc, char **argv)
{
  int i;
  params result;

  result.disable_color_correction = 0;
  result.disable_cutting_out_document = 0;
  result.save_preview = 0;
  result.error = 0;
  result.print_help = 0;
  result.read_photo_from_file = 0;

  for (i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "-i") == 0)
    {
      if (i + 1 < argc)
      {
        result.input_file = argv[i + 1];
        result.read_photo_from_file = 1;
      }
      else
      {
        result.error = 1;
        result.print_help = 1;
        return result;
      }
    }
    else if (strcmp(argv[i], "-o") == 0)
    {
      if (i + 1 < argc)
        result.output_file = argv[i + 1];
      else
      {
        result.error = 1;
        result.print_help = 1;
        return result;
      }
    }
    else if (strcmp(argv[i], "-h") == 0 ||
             strcmp(argv[i], "--help") == 0 ||
             strcmp(argv[i], "?") == 0)
    {
      result.print_help = 1;
    }
    else if (strcmp(argv[i], "-preview") == 0)
      result.save_preview = 1;
    else if (strcmp(argv[i], "-no-cutout") == 0)
      result.disable_cutting_out_document = 1;
    else if (strcmp(argv[i], "-no-col-corr") == 0)
      result.disable_color_correction = 1;
  }

  if (result.output_file.empty())
    result.error = 1;

  return result;
}

void print_help(char * argv_0)
{
  std::cout << "Document Scanner 2020 @ Marszaluk Lukasz Wolny Dominik\n";
  std::cout << "Simple software for book / document embedded scanners.\n";
  std::cout << "Sample usage:\n   ";
  std::cout << argv_0 << " -o OUTPUT_FILENAME [-i INPUT_FILENAME] ";
  std::cout << "[-preview] [-no-cutout] [-no-col-corr] [--help]\n";
  std::cout << "Obligatory:\n";
  std::cout << "  -o OUTPUT_FILENAME - path to output file\n";
  std::cout << "Optional: []\n";
  std::cout << "  -i INPU_FILENAME - load photo from file instead making them\n";
  std::cout << "  -preview - save preview photo (without modifications)\n";
  std::cout << "  -no-cutout - do not cutout background from photo\n";
  std::cout << "  -no-col-corr - do not improve colors of output photo (contrast, while balance)\n";
  std::cout << "Add -h; --help; ? to print this message\n";
}

void print_message (std::string msg)
{
  std::cout << msg;
}