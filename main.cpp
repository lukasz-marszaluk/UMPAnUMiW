#include "api/camera_api.hpp"

#include "inc/image.hpp"
#include "inc/document.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    
    std::string output_filename;
    std::string input_filename;
    
    image photo;
    document doc;

    bool read_photo_from_file;

    // recognize arguments
    // print help

    if (read_photo_from_file)
        photo = image(input_filename);
    else
        photo = take_photo();



    // recognize document inside photo
    // cut out and stretch document
    // imporove visibility

    return 0;
}
