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

	if (bw_img->last_result == OK)
	{
		separate_objects_from_image(bw_img, separated_image);
		biggest_object_index = find_biggest_object(separated_image);

		for (i = 0; i < img->width * img->height; i++)
			separated_image[i] = (separated_image[i] == biggest_object_index);

		exact_document_corners(separated_image);
	}
	else
	{
		img->last_result = UNDEFINED;
	}

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
	doc_corners[0] = resolve_equations(&(lines[3]), &(lines[0]));
	doc_corners[1] = resolve_equations(&(lines[0]), &(lines[1]));
	doc_corners[2] = resolve_equations(&(lines[2]), &(lines[3]));
	doc_corners[3] = resolve_equations(&(lines[2]), &(lines[1]));
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
		slope = tan(angle);

		ln->slope = slope;
		ln->vertical_shift = ln->y - ln->slope * ln->x;

		matching_index = check_line_match(separated_image, ln);

		if (best_matching_index < matching_index)
		{
			best_matching_index = matching_index;
			best_slope = ln->slope;
		}
	}

	ln->slope = best_slope;
	ln->vertical_shift = ln->y - ln->slope * ln->x;
}

int document::check_line_match(int *separated_image, line *ln)
{
	int xi, yi;
	int yi_beg, yi_end, xi_beg, xi_end;
	int matching_points = 0;

	if (ln->slope == 0.0)
	{
		yi_beg = ln->y - 2;
		yi_end = ln->y + 2;

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
		xi_beg = ln->x - 2;
		xi_end = ln->x + 2;

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

		yi_beg = yi - 2;
		yi_end = yi + 2;

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

point document::resolve_equations(line *first_line, line *second_line)
{
	int x, y;
	double w, w_x, w_y;
	if (first_line->slope == 0.0)
	{
		if (second_line->slope == tan(-M_PI_2))
			return {second_line->x, first_line->y};

		y = first_line->y;
		x = (double)(y - second_line->vertical_shift) / second_line->slope;

		return {x, y};
	}
	else if (first_line->slope == tan(-M_PI_2))
	{
		if (second_line->slope == 0.0)
			return {first_line->x, second_line->y};

		x = first_line->x;
		y = x * second_line->slope + second_line->vertical_shift;

		return {x, y};
	}
	else if (second_line->slope == tan(-M_PI_2))
	{
		x = second_line->x;
		y = x * first_line->slope + first_line->vertical_shift;

		return {x, y};
	}
	else if (second_line->slope == 0.0)
	{
		y = second_line->y;
		x = (double)(y - first_line->vertical_shift) / first_line->slope;

		return {x, y};
	}
	else
	{
		x = (int((double)(second_line->vertical_shift - first_line->vertical_shift) /
								 (first_line->slope - second_line->slope) +
						 0.5));

		y = (int)(first_line->slope * x + first_line->vertical_shift);

		return {x, y};
	}
}

void document::stretch_document()
{
	int i;

	if (img->last_result != OK)
		return;

	for (i = 0; i < 4; i++)
	{
		if (doc_corners[i].x < 0)
			doc_corners[i].x = 1;

		if (doc_corners[i].x - 2 > img->width)
			doc_corners[i].x = img->width - 2;

		if (doc_corners[i].y < 0)
			doc_corners[i].y = 1;

		if (doc_corners[i].y - 2 > img->height)
			doc_corners[i].y = img->height - 2;
	}

	img->unified_transform(doc_corners, 4);
}

void document::improve_colors()
{
	if (document_is_bw())
		remove_tint();

	improve_contrast();
}

void document::improve_contrast()
{
	int i;
	// improve contrast
	unsigned char contrast_lut[256];
	int maximum, threshold;
	int contrast_begin, contrast_end;

	int *value_histogram = img->get_histogram();

	maximum = 0;

	for (i = 0; i < 256; i++)
	{
		if (maximum < value_histogram[i])
			maximum = value_histogram[i];
	}

	threshold = maximum / 20; // 2% of maximum number of pixels for brightness

	contrast_begin = 0;
	for (i = 0; i < 256; i++)
	{
		if (value_histogram[i] > threshold)
		{
			contrast_begin = i;
			break;
		}
	}

	contrast_end = 0;
	for (i = 255; i >= 0; i--)
	{
		if (value_histogram[i] > threshold)
		{
			contrast_end = i;
			break;
		}
	}

	contrast_begin /= 2; // to save more whites

	for (i = 0; i < contrast_begin; i++)
		contrast_lut[i] = 0;

	for (i = contrast_begin; i < contrast_end; i++)
		contrast_lut[i] = 255 * (i - contrast_begin) / (contrast_end - contrast_begin);

	for (i = contrast_end; i < 256; i++)
		contrast_lut[i] = 255;

	img->apply_lookup_tables(contrast_lut, contrast_lut, contrast_lut);

	delete[] value_histogram;
}

void document::remove_tint()
{
	int i, ii, value;
	int avg_diff_red_green = 0;
	int avg_diff_red_blue = 0;

	double avg [3];
	int * histogram;

	unsigned char *lut_red = new unsigned char[256];
	unsigned char *lut_green = new unsigned char[256];
	unsigned char *lut_blue = new unsigned char[256];

	for (i = 0; i < 3; i++)
	{
		histogram = img->get_histogram (i);

		avg[i] = 0;

		for (ii = 0; ii < 256; ii++)
			avg [i] += histogram [ii] * ii;
		
		avg [i] = avg [i] / (img->width * img->height);

		delete [] histogram;
	}

	avg_diff_red_blue = avg[0] - avg [2];
	avg_diff_red_green = avg[0] - avg [1];

	for (i = 0; i < 256; i++)
	{
		lut_red[i] = i;

		value = i + avg_diff_red_green;
		if (value > 255)
			value = 255;
		if (value < 0)
			value = 0;

		lut_green[i] = value;

		value = i + avg_diff_red_blue;
		if (value > 255)
			value = 255;
		if (value < 0)
			value = 0;

		lut_blue[i] = value;
	}

	img->apply_lookup_tables(lut_red, lut_green, lut_blue);

	delete[] lut_red;
	delete[] lut_green;
	delete[] lut_blue;
}

bool document::document_is_bw()
{
	int *histogram_red = img->get_histogram(0);
	int *histogram_green = img->get_histogram(1);
	int *histogram_blue = img->get_histogram(2);

	int std_dev_red_green =
			calculate_std_dev_between_channels(histogram_red, histogram_green);

	int std_dev_red_blue =
			calculate_std_dev_between_channels(histogram_red, histogram_blue);

	delete[] histogram_red;
	delete[] histogram_green;
	delete[] histogram_blue;

	return (std_dev_red_blue < 40 && std_dev_red_green < 40);
}

int document::calculate_std_dev_between_channels(int *histogram_a, int *histogram_b)
{
	int i, pow;

	int sum_diff_a_b;
	int avg_diff_a_b;
	unsigned int std_dev_a_b;

	sum_diff_a_b = 0;
	for (i = 0; i < 256; i++)
		sum_diff_a_b += histogram_a[i] - histogram_b[i];

	avg_diff_a_b = sum_diff_a_b / (img->width * img->height);

	std_dev_a_b = 0;
	for (i = 0; i < 256; i++)
	{
		pow = histogram_a[i] - histogram_b[i] - avg_diff_a_b;
		std_dev_a_b += pow * pow;
	}

	return sqrt((std_dev_a_b) / (img->width * img->height));
}