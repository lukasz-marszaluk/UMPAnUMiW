#include "api/camera_api.hpp"

#include "inc/image.hpp"
#include "inc/document.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    
    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    
    image *photo;
    // document doc;

    bool read_photo_from_file = 1;

    // recognize arguments
    // print help

    if (read_photo_from_file)
        photo = new image(input_filename);
    else
        photo = take_photo();


    photo->gaussian_blur();
    photo->save(output_filename);

    // recognize document inside photo
    // cut out and stretch document
    // imporove visibility

    return 0;
}
