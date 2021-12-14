#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define	STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include "OOP_Image.h"
#include "FP_Image.h"
using namespace std::literals;


int time(std::vector<std::chrono::nanoseconds> duration)
{
	if (duration.size() == 0) return 0;
	std::chrono::nanoseconds time = 0ns;
	for (auto& i : duration)
	{
		time += i;
	}
	return std::chrono::duration_cast<std::chrono::milliseconds>(time).count() / duration.size();
}

std::pair<int, int>GetTimings(const char* filePath, int loops = 100)
{
	int x, y, channels;

	std::vector<std::chrono::nanoseconds> OOP_duration, FP_duration;

	std::string path = ".\\images\\";
	path += filePath;

	unsigned char* img = stbi_load(path.c_str(), &x, &y, &channels, 0);
	if (img == NULL)
	{
		printf("Failed to load image");
		return std::pair<int, int>();
	}
	printf("Loaded image with %dpx width, %dpx height, and %d channels.\n", x, y, channels);

	unsigned char* OOP_img;

	for (int i = 0; i < loops; ++i)
	{
		printf("OOP %dx%d Loop:\t%d\n", x, y, i);
		auto start = std::chrono::high_resolution_clock::now();
		OOP_Image* oop_image = new OOP_Image(img, x, y, channels);
		OOP_img = (*oop_image).applyFilter(3);
		auto end = std::chrono::high_resolution_clock::now() - start;
		OOP_duration.push_back(end);
		free(OOP_img);
		delete oop_image;
	}

	printf("\n");

	unsigned char* FP_img;

	for (int i = 0; i < loops; ++i)
	{
		printf("FP %dx%d Loop:\t%d\n", x, y, i);
		auto start = std::chrono::high_resolution_clock::now();
		auto v_img = VectoriseImage(img, x, y, channels);
		FP_img = GaussianBlur2(v_img, x, 3);
		auto end = std::chrono::high_resolution_clock::now() - start;
		FP_duration.push_back(end);
		free(FP_img);
	}

	return std::pair<int, int>(time(OOP_duration), time(FP_duration));
}

int main() 
{
	std::vector<std::pair<std::pair<int, int>, float>> timings;

	const char* fourK = "4000x6000.jpg";
	const char* twoK = "2000x3000.jpg";
	const char* oneK = "1000x1500.jpg";
	const char* halfK = "500x750.jpg";

	timings.push_back(std::pair<std::pair<int, int>, float>((GetTimings(fourK, 100)), (4000*6000) / 1000));
	timings.push_back(std::pair<std::pair<int, int>, float>((GetTimings(twoK,  100)), (2000*3000) / 1000));
	timings.push_back(std::pair<std::pair<int, int>, float>((GetTimings(oneK,  100)), (1000*1500) / 1000));
	timings.push_back(std::pair<std::pair<int, int>, float>((GetTimings(halfK, 100)), (500*750) / 1000));

	printf("File:\t\tOOP:\tFP:\n");
	for (auto &i : timings)
	{
		printf("%f:\t%dms\t%dms\n", i.second, i.first.first, i.first.second);
	}

	printf("Latex output:\n");
	printf("{ ");
	for (auto &i : timings)
	{
		printf("(%f, %d) ", i.second, i.first.first);
	}
	printf("};\n");
	printf("{ ");
	for (auto &i : timings)
	{
		printf("(%f, %d) ", i.second, i.first.second);
	}
	printf("};");
}