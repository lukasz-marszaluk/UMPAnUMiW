#include "../inc/document.hpp"

#include <cstring>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

document::document(image *_img)
{
	img = new image(_img);
}

document::~document()
{
	if (img != NULL)
		delete img;
}

void document::recognize_document()
{
	int i;

	grayscale_image *bw_img;
	int *separated_image = new int[img->width * img->height];
	int biggest_object_index;

	bw_img = new grayscale_image(img);
	bw_img->canny_edge_detection();

	separate_objects_from_image(bw_img, separated_image);
	biggest_object_index = find_biggest_object(separated_image);

	for (i = 0; i < img->width * img->height; i++)
		separated_image[i] = (separated_image[i] == biggest_object_index);

	exact_document_corners(separated_image);

	delete[] separated_image;
	delete bw_img;
}

image *document::get_document_image()
{
	return img;
}

void document::separate_objects_from_image(grayscale_image *image, int *separated_image)
{
	int last_object_index = 0;
	int xi, yi, i;
	bool break_loops;

	for (i = 0; i < img->width * img->height; i++)
		separated_image[i] = -1;

	for (yi = 0; yi < image->height; yi++)
	{
		for (xi = 0; xi < image->width; xi++)
		{
			if (image->get_pixel(xi, yi) == 0)
				continue;

			take_out_object(image, separated_image, last_object_index, xi, yi);
			last_object_index++;
		}
	}
}

void document::take_out_object(grayscale_image *image, int *separated_image, int object_index, int x, int y)
{
	std::vector<point> object_queue;
	point current_point;
	int xi, yi;

	object_queue.push_back({x, y});

	while (object_queue.size() > 0)
	{
		current_point = object_queue.back();
		x = current_point.x;
		y = current_point.y;
		object_queue.pop_back();

		image->set_pixel(current_point.x, current_point.y, 0);
		separated_image[current_point.y * image->width + current_point.x] = object_index;

		for (yi = -3; yi <= 3; yi++)
		{
			for (xi = -3; xi <= 3; xi++)
			{
				if ((x + xi < 0) || (x + xi >= image->width))
					continue;

				if ((y + yi < 0) || (y + yi >= image->height))
					continue;

				if (image->get_pixel(x + xi, y + yi) == 255)
					object_queue.push_back({x + xi, y + yi});
			}
		}
	}
}

int document::find_biggest_object(int *separated_image)
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
			if (number_of_objects < separated_image[yi * img->width + xi])
				number_of_objects = separated_image[yi * img->width + xi];
		}
	}

	number_of_objects++;

	int *x_beg = new int[number_of_objects];
	int *x_end = new int[number_of_objects];
	int *y_beg = new int[number_of_objects];
	int *y_end = new int[number_of_objects];

	for (i = 0; i < number_of_objects; i++)
	{
		x_beg[i] = img->width;
		x_end[i] = 0;
		y_beg[i] = img->height;
		y_end[i] = 0;
	}

	for (yi = 0; yi < img->height; yi++)
	{
		for (xi = 0; xi < img->width; xi++)
		{
			currently_checked_object = separated_image[yi * img->width + xi];

			if (currently_checked_object == -1)
				continue;

			if (x_beg[currently_checked_object] > xi)
				x_beg[currently_checked_object] = xi;

			if (x_end[currently_checked_object] < xi)
				x_end[currently_checked_object] = xi;

			if (y_beg[currently_checked_object] > yi)
				y_beg[currently_checked_object] = yi;

			if (y_end[currently_checked_object] < yi)
				y_end[currently_checked_object] = yi;
		}
	}

	biggest_object_index = 0;
	biggest_object_area = 0;
	for (i = 0; i < number_of_objects; i++)
	{
		object_area = (x_end[i] - x_beg[i]) * (y_end[i] - y_beg[i]);

		if (biggest_object_area < object_area)
		{
			biggest_object_area = object_area;
			biggest_object_index = i;
		}
	}

	delete[] x_beg;
	delete[] x_end;
	delete[] y_beg;
	delete[] y_end;

	return biggest_object_index;
}

void document::exact_document_corners(int *separated_image)
{
	int xi, yi, i;
	int sum_x, sum_y, size;
	int center_x, center_y;
	int radius = 16;
	bool break_loops;

	line lines[4]; // top, right, bottom, left

	sum_x = 0;
	sum_y = 0;
	size = 0;

	for (yi = 0; yi < img->height; yi++)
	{
		for (xi = 0; xi < img->width; xi++)
		{
			if (separated_image[yi * img->width + xi])
			{
				sum_x += xi;
				sum_y += yi;
				size++;
			}
		}
	}

	center_x = (sum_x + size / 2) / size;
	center_y = (sum_y + size / 2) / size;

	if (center_x < 16)
		center_x = 16;
	if (center_x >= img->width - 16)
		center_x = img->width - 17;
	if (center_y < 16)
		center_y = 16;
	if (center_y >= img->height - 16)
		center_y = img->height - 17;

	// to top
	break_loops = false;
	for (yi = center_y; (yi >= 0 && !break_loops); yi--)
	{
		for (xi = center_x - radius; (xi < center_x + radius && !break_loops); xi++)
		{
			if (separated_image[yi * img->width + xi])
			{
				lines[0].x = xi;
				lines[0].y = yi;
				break_loops = true;
			}
		}
	}

	// to right
	break_loops = false;
	for (xi = center_x; (xi < img->width && !break_loops); xi++)
	{
		for (yi = center_y - radius; (yi < center_y + radius && !break_loops); yi++)
		{
			if (separated_image[yi * img->width + xi])
			{
				lines[1].x = xi;
				lines[1].y = yi;
				break_loops = true;
			}
		}
	}

	// to bottom
	break_loops = false;
	for (yi = center_y; (yi < img->height && !break_loops); yi++)
	{
		for (xi = center_x - radius; (xi < center_x + radius && !break_loops); xi++)
		{
			if (separated_image[yi * img->width + xi])
			{
				lines[2].x = xi;
				lines[2].y = yi;
				break_loops = true;
			}
		}
	}

	// to left
	break_loops = false;
	for (xi = center_x; (xi >= 0 && !break_loops); xi--)
	{
		for (yi = center_y - radius; (yi < center_y + radius && !break_loops); yi++)
		{
			if (separated_image[yi * img->width + xi])
			{
				lines[3].x = xi;
				lines[3].y = yi;
				break_loops = true;
			}
		}
	}

	// find best lines
	for (i = 0; i < 4; i++)
		find_best_line(separated_image, &(lines[i]));

	// find crossing points
	int temp = 0;
	// doc_corners[0] = resolve_equations(&(lines[3]), &(lines[0]));
	// doc_corners[1] = resolve_equations(&(lines[0]), &(lines[1]));
	// doc_corners[2] = resolve_equations(&(lines[2]), &(lines[3]));
	// doc_corners[1] = resolve_equations(&(lines[2]), &(lines[1]));
}

void document::find_best_line(int *separated_image, line *ln)
{
	double angle;
	double slope;
	int i;

	double best_slope = 0.0;
	int best_matching_index = 0;
	int matching_index;

	for (i = 0; i < 100; i++)
	{
		angle = M_PI * (i - 50) / 100;
		slope = tan (angle);

		ln->slope = slope;
		ln->vertical_shift = ln->y - ln->slope * ln->x;

		matching_index = check_line_match (separated_image, ln);

		if (best_matching_index < matching_index)
		{
			best_matching_index = matching_index;
			best_slope = ln->slope;
		}
	}

	ln->slope = best_slope;
}

int document::check_line_match(int *separated_image, line *ln)
{
	int xi, yi;
	int yi_beg, yi_end, xi_beg, xi_end;
	int matching_points = 0;

	if (ln->slope == 0.0)
	{
		yi_beg = ln->y - 8;
		yi_end = ln->y + 8;

		if (yi_beg < 0)
			yi_beg = 0;

		if (yi_end > img->height)
			yi_end = img->height - 1;

		for (yi = yi_beg; yi < yi_end; yi++)
			for (xi = 0; xi < img->width; xi++)
				if (separated_image[yi * img->width + xi])
					matching_points++;

		return matching_points;
	}

	if (ln->slope == tan(-M_PI_2))
	{
		xi_beg = ln->x - 8;
		xi_end = ln->x + 8;

		if (xi_beg < 0)
			xi_beg = 0;

		if (xi_end > img->width)
			xi_end = img->width - 1;

		for (yi = 0; yi < img->height; yi++)
			for (xi = xi_beg; xi < xi_end; xi++)
				if (separated_image[yi * img->width + xi])
					matching_points++;

		return matching_points;
	}

	for (xi = 0; xi < img->width; xi++)
	{
		yi = ln->slope * xi + ln->vertical_shift;

		yi_beg = yi - 8;
		yi_end = yi + 8;

		if (yi_beg < 0)
			yi_beg = 0;

		if (yi_end > img->height)
			yi_end = img->height - 1;

		for (yi = yi_beg; yi < yi_end; yi++)
			if (separated_image[yi * img->width + xi])
				matching_points++;
	}

	return matching_points;
}
