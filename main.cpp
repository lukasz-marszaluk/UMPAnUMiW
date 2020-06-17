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

    grayscale_image *bw_photo = new grayscale_image(photo);

    bw_photo->canny_edge_detection();

    delete photo;
    photo = new image(bw_photo);
    photo->save(output_filename);
    delete photo;
    // recognize document inside photo
    // cut out and stretch document
    // imporove visibility

    return 0;
}
