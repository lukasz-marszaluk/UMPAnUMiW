#include "camera_api.hpp"

image* take_photo ()
{
    image *fake = new image ();
    return fake;
}

void delete_photo (image *photo)
{
	delete photo;
}
