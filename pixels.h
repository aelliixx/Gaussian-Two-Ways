#pragma once
#include <cstdint>

struct pixels
{
	uint16_t R = 0;
	uint16_t G = 0;
	uint16_t B = 0;

	struct pixels& operator+=(const pixels const& rhs)
	{
		R += rhs.R;
		G += rhs.G;
		B += rhs.B;
		return *this;
	}

	struct pixels& operator/=(const uint16_t& rhs)
	{
		R /= rhs;
		G /= rhs;
		B /= rhs;
		return *this;
	}

	struct pixels& operator*=(const uint16_t& rhs)
	{
		R *= rhs;
		G *= rhs;
		B *= rhs;
		return *this;
	}

	const pixels operator+(const pixels const& rhs) const
	{
		return pixels{ (uint16_t)(R + rhs.R), (uint16_t)(G + rhs.G),
			(uint16_t)(B + rhs.B) };
	}

	const pixels operator*(const uint16_t const& rhs) const
	{
		return pixels{ (uint16_t)(R * rhs), (uint16_t)(G * rhs),
			(uint16_t)(B * rhs) };
	}

	const pixels operator/(const uint16_t const& rhs) const
	{
		return pixels{ (uint16_t)(R / rhs), (uint16_t)(G / rhs),
			(uint16_t)(B / rhs) };
	}

	bool operator==(pixels const& rhs) const
	{
		return (this->R == rhs.R && this->B == rhs.B && this->G == rhs.G);
	}
};

