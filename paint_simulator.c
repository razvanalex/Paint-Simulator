//----------------------------------------------------------------
//  paint_simulator.c
//  Smadu Razvan-Alexandru 315CB
//----------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>

//Constants
#define MIN_NUM_PX	1
#define MAX_NUM_PX	1024
#define MAX_COLOR_PX	255

//Errors
#define EPERM	1
#define EINVAL	2
#define ENOMEM	3


typedef struct
{
	unsigned char r, g, b;
} pixel;

int Is_Between_Min_Max(int Number, int Min, int Max)
{
	if (Number >= Min && Number <= Max)
		return 1;
	return 0;
}

pixel** Initialize_Image(unsigned int width, unsigned int height)
{
	unsigned int i, j;
	pixel **image = malloc(height * sizeof(pixel*));

	//If it couldn't alloc memory then return null
	if (image == NULL)
		return NULL;

	for (i = 0; i < height; i++)
	{
		image[i] = calloc(width, sizeof(pixel));

		//If it couldn't alloc memory then return null
		if (image[i] == NULL)
		{
			for (j = 0; j < i; j++)
				free(image[i]);
			free(image);
			return NULL;
		}
	}

	return image;
}

void Free_Image(pixel ***image, unsigned int height)
{
	unsigned int i;
	for (i = 0; i < height; i++)
	{
		free((*image)[i]);
	}
	free(*image);
	*image = NULL;
}

void Create_Image(pixel **image, unsigned int width, unsigned int height)
{
	unsigned int i, j;

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			scanf("%hhu%hhu%hhu", &image[i][j].r,
					      &image[i][j].g,
					      &image[i][j].b);

			//Check the color of pixels if it's between 0 and 255
			if (!Is_Between_Min_Max(image[i][j].r,
						0, MAX_COLOR_PX) ||
			    !Is_Between_Min_Max(image[i][j].g,
			    			0, MAX_COLOR_PX) ||
			    !Is_Between_Min_Max(image[i][j].b,
			    			0, MAX_COLOR_PX))
			{
				fprintf(stderr, "%d\n", EINVAL);
				Free_Image(&image, height);
				exit(EXIT_FAILURE);
			}
		}
	}
}

void Print_Image(pixel **image, unsigned int width, unsigned int height)
{
	unsigned int i, j;

	printf("%d %d\n", width, height);

	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			printf("%hhu %hhu %hhu ", image[i][j].r,
						  image[i][j].g,
						  image[i][j].b);
		}
		printf("\n");
	}

	//Fix printf to stderr
	fflush(stdout);
}

void Crop_Image(pixel ***image, unsigned int *width, unsigned int *height)
{
	unsigned int i, j;
	unsigned int start_X, start_Y, end_X, end_Y;
	unsigned int new_width, new_height;
	pixel **tmp_img;

	scanf("%d%d%d%d", &start_X, &start_Y, &end_X, &end_Y);

	//Check input requirements
	if (!Is_Between_Min_Max(start_X, 0, end_X) ||
	    !Is_Between_Min_Max(end_X, 0, *width)  ||
	    !Is_Between_Min_Max(start_Y, 0, end_Y) ||
	    !Is_Between_Min_Max(end_Y, 0, *height))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(image, *height);
		exit(EXIT_FAILURE);
	}

	//Create a temporary image which stores the new image created
	new_width = end_X - start_X + 1;
	new_height = end_Y - start_Y + 1;
	tmp_img = Initialize_Image(new_width, new_height);

	if (tmp_img == NULL)
	{
		fprintf(stderr, "%d\n", ENOMEM);
		Free_Image(image, *height);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < new_height; i++)
	{
		for (j = 0; j < new_width; j++)
		{
			tmp_img[i][j] = (*image)[i + start_Y][j + start_X];
		}
	}

	//Release unecessary memory and point *image to the new picture
	Free_Image(image, *height);
	*image = tmp_img;
	*width = new_width;
	*height = new_height;
	tmp_img = NULL;
}

void Resize_Image(pixel ***image, unsigned int *width, unsigned int *height)
{
	unsigned int i, j;
	unsigned int new_width, new_height;
	unsigned int min_width, min_height;
	pixel **tmp_img;

	scanf("%d%d", &new_width, &new_height);

	//Check if new width and height are between 1 and 1024
	if (!Is_Between_Min_Max(new_height, MIN_NUM_PX, MAX_NUM_PX) ||
	    !Is_Between_Min_Max(new_width, MIN_NUM_PX, MAX_NUM_PX))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(image, *height);
		exit(EXIT_FAILURE);
	}

	//Create a temporary image which stores the new image created
	tmp_img = Initialize_Image(new_width, new_height);

	if (tmp_img == NULL)
	{
		fprintf(stderr, "%d\n", ENOMEM);
		Free_Image(image, *height);
		exit(EXIT_FAILURE);
	}

	//Calculate width and height from old image
	//which match with new dimensions
	min_width = (new_width < *width) ? new_width : *width;
	min_height = (new_height < *height) ? new_height : *height;

	//Resize image and make new pixels white
	for (i = 0; i < new_height; i++)
	{
		for (j = 0; j < new_width; j++)
		{
			if (i < min_height && j < min_width)
				tmp_img[i][j] = (*image)[i][j];
			else
			{
				tmp_img[i][j].r = MAX_COLOR_PX;
				tmp_img[i][j].g = MAX_COLOR_PX;
				tmp_img[i][j].b = MAX_COLOR_PX;
			}
		}
	}

	//Release unecessary memory and point *image to the new picture
	Free_Image(image, *height);
	*image = tmp_img;
	*width = new_width;
	*height = new_height;
	tmp_img = NULL;

}

void Color_Region(pixel **image, unsigned int width, unsigned int height)
{
	unsigned int i, j;
	unsigned int start_X, start_Y, end_X, end_Y;
	unsigned int color_reg_width, color_reg_height;
	pixel color;

	scanf("%d%d%d%d", &start_X, &start_Y, &end_X, &end_Y);

	//Check coordinates are in the picture
	if (!Is_Between_Min_Max(start_X, 0, end_X) ||
	    !Is_Between_Min_Max(end_X, 0, width)   ||
	    !Is_Between_Min_Max(start_Y, 0, end_Y) ||
	    !Is_Between_Min_Max(end_Y, 0, height))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(&image, height);
		exit(EXIT_FAILURE);
	}

	color_reg_width = end_X - start_X + 1;
	color_reg_height = end_Y - start_Y + 1;

	scanf("%hhu%hhu%hhu", &color.r, &color.g, &color.b);

	//Check the color of pixel if it's between 0 and 255
	if (!Is_Between_Min_Max(color.r, 0, MAX_COLOR_PX) ||
	    !Is_Between_Min_Max(color.g, 0, MAX_COLOR_PX) ||
	    !Is_Between_Min_Max(color.b, 0, MAX_COLOR_PX))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(&image, height);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < color_reg_height; i++)
	{
		for (j = 0; j < color_reg_width; j++)
		{
			image[i + start_Y][j + start_X] = color;
		}
	}
}

void Blur_Image(pixel ***image, unsigned int width, unsigned int height)
{
	unsigned int i, j;
	unsigned int sum_r, sum_g, sum_b;
	unsigned int num_sum;
	unsigned int iter, num_iter;
	pixel **blur_img, **tmp_img;

	blur_img = Initialize_Image(width, height);

	if (blur_img == NULL)
	{
		fprintf(stderr, "%d\n", ENOMEM);
		Free_Image(image, height);
		exit(EXIT_FAILURE);
	}

	tmp_img = *image;

	scanf("%d", &num_iter);

	//Check if blur factor is less than 2000
	if (!Is_Between_Min_Max(num_iter, 0, 2000))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(image, height);
		exit(EXIT_FAILURE);
	}

	//Calculate blured pixels
	for (iter = 0; iter < num_iter; iter++)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				sum_r = sum_g = sum_b = 0;
				num_sum = 0;
				if (i < height - 1)
				{
					sum_r += image[i + 1][j]->r;
					sum_g += image[i + 1][j]->g;
					sum_b += image[i + 1][j]->b;
					num_sum++;
				}
				if (i > 0)
				{
					sum_r += image[i - 1][j]->r;
					sum_g += image[i - 1][j]->g;
					sum_b += image[i - 1][j]->b;
					num_sum++;
				}
				if (j < width - 1)
				{
					sum_r += image[i][j + 1]->r;
					sum_g += image[i][j + 1]->g;
					sum_b += image[i][j + 1]->b;
					num_sum++;
				}
				if (j > 0)
				{
					sum_r += image[i][j - 1]->r;
					sum_g += image[i][j - 1]->g;
					sum_b += image[i][j - 1]->b;
					num_sum++;
				}

				blur_img[i][j].r = sum_r / num_sum;
				blur_img[i][j].g = sum_g / num_sum;
				blur_img[i][j].b = sum_b / num_sum;
			}
		}

		//Store blured image in *image
		*image = blur_img;
		blur_img = tmp_img;
		tmp_img = *image;
	}

	//Release memory
	Free_Image(&blur_img, height);
	blur_img = NULL;
	tmp_img = NULL;
}

void Rotate_Image(pixel ***image, unsigned int *width, unsigned int *height)
{
	unsigned int i, j;
	unsigned int num_rot, rot;
	unsigned int new_width, new_height;
	pixel **tmp_img;

	scanf("%d", &num_rot);

	if (!Is_Between_Min_Max(num_rot, 1, 3))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(image, *height);
		exit(EXIT_FAILURE);
	}

	//Rotate image pixel by 'num_rot' times
	for (rot = 0; rot < num_rot; rot++)
	{
		new_width = *height;
		new_height = *width;

		tmp_img = Initialize_Image(new_width, new_height);

		if (tmp_img == NULL)
		{
			fprintf(stderr, "%d\n", ENOMEM);
			Free_Image(image, *height);
			exit(EXIT_FAILURE);
		}

		for (i = 0; i < new_height; i++)
		{
			for (j = 0; j < new_width; j++)
			{
				tmp_img[i][j] = (*image)[new_width - j - 1][i];
			}
		}

		//Release memory and point image to rotated image
		Free_Image(image, *height);
		*image = tmp_img;
		*width = new_width;
		*height = new_height;
		tmp_img = NULL;
	}
}

void Change_Region_Color(pixel **image, unsigned int width, unsigned int height,
			 pixel old_color, pixel new_color,
			 unsigned int X, unsigned int Y)
{
	//Exit conditions
	if (image[Y][X].r == new_color.r &&
	    image[Y][X].g == new_color.g &&
	    image[Y][X].b == new_color.b)
		return;

	if (image[Y][X].r != old_color.r ||
	    image[Y][X].g != old_color.g ||
	    image[Y][X].b != old_color.b)
		return;

	//Color pixel
	image[Y][X] = new_color;

	//Move to next neighbor pixel
	if (X < width - 1)
		Change_Region_Color(image, width, height,
				    old_color, new_color,
				    X + 1, Y);
	if (X > 0)
		Change_Region_Color(image, width, height,
				    old_color, new_color,
				    X - 1, Y);
	if (Y < height - 1)
		Change_Region_Color(image, width, height,
				    old_color, new_color,
				    X, Y + 1);
	if (Y > 0)
		Change_Region_Color(image, width, height,
				    old_color, new_color,
				    X, Y - 1);
}

void Fill_Region(pixel **image, unsigned int width, unsigned int height)
{
	unsigned int start_X, start_Y;
	pixel color, old_color;

	scanf("%d%d", &start_X, &start_Y);

	//Check coordinates are in picture
	if (!Is_Between_Min_Max(start_X, 0, width) ||
	    !Is_Between_Min_Max(start_Y, 0, height))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(&image, height);
		exit(EXIT_FAILURE);
	}

	scanf("%hhu%hhu%hhu", &color.r, &color.g, &color.b);

	//Check the color of pixel if it's between 0 and 255
	if (!Is_Between_Min_Max(color.r, 0, MAX_COLOR_PX) ||
	    !Is_Between_Min_Max(color.g, 0, MAX_COLOR_PX) ||
	    !Is_Between_Min_Max(color.b, 0, MAX_COLOR_PX))
	{
		fprintf(stderr, "%d\n", EINVAL);
		Free_Image(&image, height);
		exit(EXIT_FAILURE);
	}

	old_color = image[start_Y][start_X];

	Change_Region_Color(image, width, height,
			    old_color, color,
			    start_X, start_Y);
}

int main()
{
	unsigned int width, height;
	pixel **image = NULL;
	char op_code;

	do{
		scanf("%hhu", &op_code);

		switch (op_code)
		{
		case 1:
			//If there is an image already stored, 
			//it will be erased from memory
			if (image != NULL)
				Free_Image(&image, height);

			scanf("%u%u/n", &width, &height);

			//Check if width and height are between 1 and 1024
			if (!Is_Between_Min_Max(height, 
						MIN_NUM_PX, MAX_NUM_PX) ||
			    !Is_Between_Min_Max(width, 
						MIN_NUM_PX, MAX_NUM_PX))
			{
				fprintf(stderr, "%d\n", EINVAL);
				exit(EXIT_FAILURE);
			}

			//Initialize and create image
			image = Initialize_Image(width, height);
			if (image == NULL)
			{
				fprintf(stderr, "%d\n", ENOMEM);
				exit(EXIT_FAILURE);
			}
			Create_Image(image, width, height);
			break;
		case 2:
			if (image != NULL)
				Crop_Image(&image, &width, &height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 3:
			if (image != NULL)
				Resize_Image(&image, &width, &height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 4:
			if (image != NULL)
				Color_Region(image, width, height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 5:
			if (image != NULL)
				Blur_Image(&image, width, height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 6:
			if (image != NULL)
				Rotate_Image(&image, &width, &height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 7:
			if (image != NULL)
				Fill_Region(image, width, height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 8:
			if (image != NULL)
				Print_Image(image, width, height);
			else
			{
				fprintf(stderr, "%d\n", EPERM);
				exit(EXIT_FAILURE);
			}
			break;
		case 0:
			break;
		default:
			fprintf(stderr, "%d\n", EPERM);
			exit(EXIT_FAILURE);
			break;
		}

	} while (op_code != 0);

	//Free memory
	if (image != NULL)
		Free_Image(&image, height);

	return 0;
}
