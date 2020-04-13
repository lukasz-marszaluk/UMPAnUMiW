#include "api/camera_api.hpp"
#include "inc/image.hpp"

#include <iostream>

int main(int, char**) 
{
    image *photo = new image ();

    photo->hello_world();

    take_photo();

    std::cout << "Hello, world from main!\n";

    delete photo;
}
