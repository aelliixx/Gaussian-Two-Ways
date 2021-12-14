#include "pixels.h"

bool operator==(pixels const& lhs, pixels const& rhs)
{
	return (lhs.R == rhs.R && lhs.B == rhs.B && lhs.G == rhs.G);
}