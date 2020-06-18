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

    point matrix [4];

    matrix[0] = {397, 357};
    matrix[1] = {1805, 85};
    matrix[2] = {394, 1376};
    matrix[3] = {2012, 1192};

    photo->unified_transform (matrix, 4);

    photo->save(output_filename);
    delete photo;

    return 0;
}
