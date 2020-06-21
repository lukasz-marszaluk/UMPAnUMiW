#include "api/camera_api.hpp"
#include "inc/io.hpp"

#include "inc/image.hpp"
#include "inc/document.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    params input_params;
    image *photo;
    document *doc;
    
    
    input_params = read_arguments (argc, argv);

    if (input_params.print_help)
        print_help(argv[0]);

    if (input_params.error)
    {
        print_help(argv[0]);
        return -1;
    }

    if (input_params.read_photo_from_file)
        photo = new image(input_params.input_file);
    else
        photo = take_photo();
    
    if (input_params.save_preview)
        photo->save ("./preview.jpg");

    doc = new document(photo);

    if (!input_params.disable_cutting_out_document)
    {
        doc->recognize_document();
        doc->stretch_document();
    }
    
    if (!input_params.disable_color_correction)
        doc->improve_colors();

    doc->get_document_image()->save(input_params.output_file);
    
    if (input_params.read_photo_from_file)
        delete photo;
    else
        delete_photo (photo);
    

    print_message("Done\n");
    return 0;
}
