#include "api/camera_api.hpp"

#include "inc/image.hpp"
#include "inc/document.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    std::string input_filename = argv[1];
    std::string output_filename = argv[2];
    
    image *photo = new image(input_filename);
    document *doc = new document(photo);

    doc->recognize_document();
    doc->stretch_document();

    delete photo;
    photo = doc->get_document_image();
    photo->save(output_filename);
    delete photo;

    return 0;
}
