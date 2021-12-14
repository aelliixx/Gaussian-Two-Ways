#pragma once
#include <cstdint>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include "pixels.h"



std::vector<pixels> VectoriseImage(unsigned char* image, const int x, const int y, const int channel);

inline int KernelStepSize(const int kernel);

unsigned char* ReturnImage(const std::vector<pixels> image, const size_t size, const int channel);

unsigned char* ReturnImage(const std::vector<pixels> &image, const int &it,
	const size_t size, const int &channel, unsigned char* const & pg, unsigned char* &gaussian);

std::vector<pixels> Blur(const std::vector<pixels>& px, const int width,
	const int channels, const int kernelSize = 3);

unsigned char* GaussianBlur(const std::vector<pixels>& image, const int width, 
	const int channel, const int kernelSize = 3);


unsigned char* GaussianBlur2(const std::vector<pixels>& image, const int width,	const int kernelSize = 3);


