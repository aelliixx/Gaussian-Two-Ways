#include "OOP_Image.h"

OOP_Image::OOP_Image(unsigned char* image, const int x, const int y, const int channels)
	: channels{ channels }, width{ x }, height{ y }
{
	size = width * height * channels;
	for (unsigned char* px = image; px != image + size; px += channels)
	{
		img.push_back(pixels{ (uint8_t)*px, (uint8_t) * (px + 1), (uint8_t) * (px + 2) });
	}
}

void OOP_Image::blur(int kernel)
{
	auto step = (int)(kernel / 2);

	for (size_t i = 0; i < img.size(); ++i)
	{
		uint16_t r = 0;
		uint16_t b = 0;
		uint16_t g = 0;

		int kernelSum = 0;
		for (auto row = step * -1; row <= step; ++row)
		{
			for (auto column = step * -1; column <= step; ++column)
			{
				size_t index = i + (width * row) + column;


				if (index >= 0 && index < img.size())
				{
					const int kernel = abs(row * column) + (4 * (!row && !column)) + (2 * (row || column) && (row != column));
					r += img[index].R * kernel;
					g += img[index].G * kernel;
					b += img[index].B * kernel;

					kernelSum += kernel;
				}
			}
		}
		r /= kernelSum;
		g /= kernelSum;
		b /= kernelSum;
		blurred.push_back(pixels{ (uint16_t)r, (uint16_t)g, (uint16_t)b });
	}
}

unsigned char* OOP_Image::applyFilter(int kernel = 3)
{
	blur(kernel);
	unsigned char* finalImage = (unsigned char*)malloc(size);
	if (finalImage == NULL)
		return nullptr;

	unsigned char* pg = finalImage;
	for (size_t i = 0; i < blurred.size(); ++i)
	{
		*pg = blurred[i].R;
		*(pg + 1) = blurred[i].G;
		*(pg + 2) = blurred[i].B;
		pg += channels;
	}
	return finalImage;
}