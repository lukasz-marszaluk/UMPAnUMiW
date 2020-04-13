#include "camera_api.hpp"

#include "../inc/image.hpp"

#include <iostream>

int take_photo ()
{
    image *win = new image ();

    std::cout << "Hello world from Windows fake lib!\t";
    win->hello_world();
    delete win;
    return 0;
}