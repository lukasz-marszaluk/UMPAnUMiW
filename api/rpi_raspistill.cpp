#include "camera_api.hpp"

image* take_photo ()
{
	system ("sudo raspistill -o photo.jpg -e");

	std::string name = "photo.jpg";
	image *photo = new image(name);

	system("sudo rm photo.jpg");	

	return photo;
}

void delete_photo (image *photo)
{
	delete photo;
}
