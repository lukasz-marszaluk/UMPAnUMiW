#include "../inc/document.hpp"

#include <cstring>

document::document (image* _img)
{
	img = new image (_img);
}

document::~document ()
{
	if (img != NULL)
		delete img;
}

void document::recognize_document ()
{
	int xi, yi;
	unsigned char pixel;

	grayscale_image* bw_img;
	int* separated_image = new int [img->width * img->height];
	int biggest_object_index;

	bw_img = new grayscale_image (img);
	bw_img->canny_edge_detection ();

	separate_objects_from_image (bw_img, separated_image);
	biggest_object_index = find_biggest_object (separated_image);

	for (yi = 0; yi < img->height; yi++)
	{
		for (xi = 0; xi < img->width; xi++)
		{
			//pixel = (unsigned char) (255 * (separated_image [yi * img->width + xi] == biggest_object_index));
			pixel = (unsigned char) (separated_image [yi * img->width + xi] % 255);

			img->set_pixel (xi, yi, pixel, pixel, pixel);
		}
	}

	delete [] separated_image;
	delete bw_img;
}

image* document::get_document_image ()
{
	return img;
}

void document::separate_objects_from_image (grayscale_image* image, int* separated_image)
{
	int last_object_index = 0;
	int xi, yi, xii, yii, i;
	bool break_loops;

	for (i = 0; i < img->width * img->height; i++)
		separated_image [i] = 0;

	for (yi = 0; yi < image->height; yi++)
	{
		for (xi = 0; xi < image->width; xi++)
		{
			if (image->get_pixel (xi, yi) == 0)
				continue;

			image->set_pixel (xi, yi, 0);

			break_loops = false;

			for (yii = -1; (yii <= 1 && !break_loops); yii++)
			{
				for (xii = -1; (xii <= 1 && !break_loops); xii++)
				{
					if ((xi + xii < 0) || (xi + xii >= image->width))
						continue;

					if ((yi + yii < 0) || (yi + yii >= image->height))
						continue;

					if (separated_image [(yi + yii) * image->width + (xi + xii)] == 0)
						continue;

					separated_image [yi * image->width + xi] =
						separated_image [(yi + yii) * image->width + (xi + xii)];

					break_loops = true;
					break;
				}
			}

			if (!break_loops)
			{
				last_object_index++;
				
				separated_image [yi * image->width + xi] = last_object_index;
			}
			
		}
	}

}

int document::find_biggest_object (int* separated_image)
{
	int xi, yi, i;
	int number_of_objects = 0;
	int currently_checked_object;

	int biggest_object_index;
	int biggest_object_area;
	int object_area;

	for (yi = 0; yi < img->height; yi++)
	{
		for (xi = 0; xi < img->width; xi++)
		{
			if (number_of_objects < separated_image [yi * img->width + xi])
				number_of_objects = separated_image [yi * img->width + xi];
		}
	}

	int* x_beg = new int [number_of_objects];
	int* x_end = new int [number_of_objects];
	int* y_beg = new int [number_of_objects];
	int* y_end = new int [number_of_objects];

	for (i = 0; i < number_of_objects; i++)
	{
		x_beg [i] = img->width;
		x_end [i] = 0;
		y_beg [i] = img->height;
		y_end [i] = 0;
	}

	for (yi = 0; yi < img->height; yi++)
	{
		for (xi = 0; xi < img->width; xi++)
		{
			currently_checked_object = separated_image [yi * img->width + xi] - 1;

			if (currently_checked_object == -1)
				continue;

			if (x_beg [currently_checked_object] > xi)
				x_beg [currently_checked_object] = xi;

			if (x_end [currently_checked_object] < xi)
				x_end [currently_checked_object] = xi;

			if (y_beg [currently_checked_object] > yi)
				y_beg [currently_checked_object] = yi;

			if (y_end [currently_checked_object] < yi)
				y_end [currently_checked_object] = yi;
		}
	}

	biggest_object_index = 0;
	biggest_object_area = 0;
	for (i = 0; i < number_of_objects; i++)
	{
		object_area = (x_end [i] - x_beg [i]) * (y_end [i] - y_beg [i]);

		if (biggest_object_area < object_area)
		{
			biggest_object_area = object_area;
			biggest_object_index = i;
		}
	}

	delete [] x_beg;
	delete [] x_end;
	delete [] y_beg;
	delete [] y_end;

	return biggest_object_index + 1;
}

unsigned int* find_biggest_object (unsigned int* separated_image);

detected_object::detected_object ()
{}

detected_object::~detected_object ()
{}
