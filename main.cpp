#include "api/camera_api.hpp"
#include "inc/image.hpp"

#include <iostream>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc != 2)
        return -1;
	
	std::cout << "Taking photo... (it could take a while)\n";
    image test = take_photo();
	std::cout << "Done.\n";

    for (int yi = 0; yi < test.height; yi++)
        for (int xi = 0; xi < test.width; xi++)
            for (int ci = 0; ci < 3; ci++)
                test.set_pixel(xi, yi, ci, test.get_pixel(xi, yi, ci));

    test.save(argv[1]);

    return 0;
}
