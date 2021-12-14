#pragma once
#include <cstdint>
#include <vector>
#include "pixels.h"

class OOP_Image
{
private:
	const int width;
	const int height;
	const int channels;
	size_t size;

	std::vector<pixels> img;
	std::vector<pixels> blurred;

public:

	OOP_Image(unsigned char* image, const int x, const int y, const int channels);

private:
	void blur(int);

public:
	unsigned char* applyFilter(int);
		
};

