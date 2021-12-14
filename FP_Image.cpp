#include "FP_Image.h"

std::vector<pixels> VectoriseImage(unsigned char* image, const int x, const int y, const int channel)
{
	std::vector<pixels> px(x * y);
	const unsigned char* i = image;
	std::generate(px.begin(), px.end(),
		[&i, &channel]() { pixels ret{
			(uint8_t)*i, (uint8_t) * (i + 1), (uint8_t) * (i + 2)
		}; i += channel; return ret; });
	return px;
}

inline int KernelStepSize(const int kernel)
{
	return (int)(kernel / 2);
}

unsigned char* ReturnImage(const std::vector<pixels> image, const size_t size, const int channel)
{
	unsigned char* const Gaussian = (unsigned char* const)malloc(size * 3);
	if (Gaussian == NULL) return NULL;
	unsigned char* pg = Gaussian;
	std::for_each(image.begin(), image.end(), [&pg, channel](const pixels& pixel) {
		*pg = pixel.R;
		*(pg + 1) = pixel.G;
		*(pg + 2) = pixel.B;
		pg += channel;
		});
	return Gaussian;
}

unsigned char* ReturnImage(const std::vector<pixels> &image, const int &it,
	const size_t size, const int &channel, unsigned char* const & pg, unsigned char* &gaussian)
{
	if (it == size)
		return gaussian;


	*pg = image[it].R;
	*(pg + 1) = image[it].G;
	*(pg + 2) = image[it].B;
	return ReturnImage(image, it + 1, size, channel, (pg + channel), gaussian);
}

[[nodiscard]] pixels GetGaussian(const std::vector<pixels>& image,
	const pixels px, const int kernelSize, const int width, const int it, const uint16_t kernelSum = 0, const int i = 0)
{
	const int grid = pow(kernelSize, 2);
	if (i == grid) return px / kernelSum;

	const int row = (int)(i / kernelSize) - 1;
	const int column = (i % kernelSize) - 1;

	const int iterator = it + (width * row) + column;
	
	if (iterator >= 0 && iterator < image.size())
	{
		uint16_t kernel = abs(row * column) + (4 * (!row && !column)) +
			(2 * (row || column) && (row != column));

		return GetGaussian(image, px + (image[iterator] * kernel), kernelSize, width, it, kernelSum + kernel,
			i + 1);
	}
	return GetGaussian(image, px, kernelSize, width, it, kernelSum, i + 1);
}

std::vector<pixels> Blur(const std::vector<pixels>& px, const int width,
	const int channels, const int kernelSize)
{	
	std::vector<pixels> Gaussian(px.size());

	int it = 0;
	std::for_each(Gaussian.begin(), Gaussian.end(), [px, kernelSize, width, &it](const pixels i) {
		return GetGaussian(px, i, kernelSize, width, it++);
		});

	return Gaussian;
}

unsigned char* GaussianBlur(const std::vector<pixels>& image, const int width, const int channel,
	const int kernelSize)
{
	return ReturnImage(Blur(image, width, channel, kernelSize), image.size(), channel);
}

unsigned char* GaussianBlur2(const std::vector<pixels>& image, const int width,
	const int kernelSize)
{
	std::vector<pixels> Gaussian(image.size());

	size_t iterator = 0;
	std::generate(Gaussian.begin(), Gaussian.end(), [image, kernelSize, &iterator, width]() {
		uint16_t r = 0;
		uint16_t b = 0;
		uint16_t g = 0;
		int kernelSum = 0;
		for (auto row = KernelStepSize(kernelSize) * -1; row <= KernelStepSize(kernelSize); ++row)
		{
			for (auto column = KernelStepSize(kernelSize)
				* -1; column <= KernelStepSize(kernelSize); ++column)
			{
				const size_t index = iterator + (width * row) + column;
				if (index >= 0 && index < image.size())
				{
					const int kernel = abs(row * column) + (4 * (!row && !column)) +
						(2 * (row || column) && (row != column));
					r += image[index].R * kernel;
					g += image[index].G * kernel;
					b += image[index].B * kernel;
					kernelSum += kernel;
				}
			}
		}
		r /= kernelSum;
		g /= kernelSum;
		b /= kernelSum;
		++iterator;
		return pixels{ (uint8_t)r, (uint8_t)g, (uint8_t)b };
		});
	return ReturnImage(Gaussian, image.size(), 3);
}

