#include <vector>

#include "doctest.h"

#include "../Convolution.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Convolution")
{
	SUBCASE("Construction by ")
	{
		Convolution<float> convolution = { 8, 3 };

		CHECK(convolution.getKernel().size() == 2);
		CHECK(convolution.getKernel()[0] == 8);
		CHECK(convolution.getKernel()[1] == 3);
	}

	SUBCASE("Construction by iterator range")
	{
		std::vector<float> kernel = { 1, 2, 3, 4 };
		
	}
}