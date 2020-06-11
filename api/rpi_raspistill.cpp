#include "camera_api.hpp"

image* take_photo ()
{
	system ("sudo raspistill -o photo.jpg -e");

	image *photo = new image("photo.jpg");

	system("sudo rm photo.jpg");	

	return photo;
}

void delete_photo (image *photo)
{
	delete photo;
}
