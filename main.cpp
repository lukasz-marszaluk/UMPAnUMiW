#include "api/camera_api.hpp"
#include "inc/image.hpp"

#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 3)
        return -1;

    take_photo();

    image test(argv[1]);

    for (int yi = 0; yi < test.height; yi++)
        for (int xi = 0; xi < test.width; xi++)
            for (int ci = 0; ci < 3; ci++)
                test.set_pixel(xi, yi, ci, test.get_pixel(xi, yi, ci));

    test.save(argv[2]);

    return 0;
}
