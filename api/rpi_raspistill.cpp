#include "camera_api.hpp"

#include "../inc/image.hpp"

#include <iostream>

int take_photo ()
{
    image *win = new image ();

    std::cout << "Hello world from Rapsberry PI raspistill lib!\t";
    win->hello_world();
    delete win;
    return 0;
}