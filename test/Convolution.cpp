#include <vector>

#include "doctest.h"

#include "../Convolution.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Convolution")
{
	SUBCASE("Construction by ")
	{
		Convolution<float> convolution = { 8, 3, 7 };

		CHECK(convolution.getKernel().size() == 3);
		CHECK(convolution.getKernel()[0] == 8);
		CHECK(convolution.getKernel()[1] == 3);
		CHECK(convolution.getKernel()[2] == 7);
	}

	SUBCASE("Construction by iterator range")
	{
		std::vector<float> kernel = { 1, 2, 3, 4 };
		Convolution<float> convolution(kernel.begin(), kernel.end());

		CHECK(convolution.getKernel() == kernel);
	}

	SUBCASE("Low-pass filter")
	{
		Convolution<float> convolution = { 0.5, 0.5 };

		CHECK(convolution(1) == doctest::Approx(0.5));
		CHECK(convolution(0) == doctest::Approx(0.5));
		CHECK(convolution(0) == doctest::Approx(0));
		CHECK(convolution(0) == doctest::Approx(0));
		CHECK(convolution(0) == doctest::Approx(0));
	}

	SUBCASE("setKernel()")
	{
		Convolution<float> convolution = { 1, 5, 13 };

		std::vector<float> kernel = { 0, 8, 0, 9, 1 };
		convolution.setKernel(kernel.begin(), kernel.end());
		CHECK(convolution.getKernel() == kernel);
	}
}