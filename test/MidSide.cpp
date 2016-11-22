#include "doctest.h"

#include "../MidSide.hpp"

using namespace dsp;

TEST_CASE("test mid-side conversion")
{
	SUBCASE("stereo")
	{
		SUBCASE("comparison")
		{
			Stereo<float> s1(1, 2);
			Stereo<float> s2(1, 2);
			REQUIRE(s1 == s2);

			s2.left = 3;
			REQUIRE(s1 != s2);
		}

		SUBCASE("stereo2ms")
		{
			SUBCASE("left == right")
			{
			    CHECK(stereo2ms<float>(0, 0) == MidSide<float>(0, 0));
			    CHECK(stereo2ms<float>(1, 1) == MidSide<float>(1, 0));
			    CHECK(stereo2ms<float>(-1, -1) == MidSide<float>(-1, 0));
			}

			SUBCASE("left != right")
			{
				CHECK(stereo2ms<float>(0, 1) == MidSide<float>(0.5, -0.5));
				CHECK(stereo2ms<float>(0, -1) == MidSide<float>(-0.5, 0.5));
				CHECK(stereo2ms<float>(1, 0) == MidSide<float>(0.5, 0.5));
				CHECK(stereo2ms<float>(-1, 0) == MidSide<float>(-0.5, -0.5));
			}
		}
	}

	SUBCASE("mid-side")
	{
		SUBCASE("comparison")
		{
			MidSide<float> ms1(1, 2);
			MidSide<float> ms2(1, 2);
			REQUIRE(ms1 == ms2);

			ms2.mid = 3;
			REQUIRE(ms1 != ms2);
		}

		SUBCASE("ms2stereo")
		{
			SUBCASE("side == 0")
			{
				CHECK(ms2stereo<float>(0, 0) == Stereo<float>(0, 0));
				CHECK(ms2stereo<float>(1, 0) == Stereo<float>(1, 1));
				CHECK(ms2stereo<float>(-1, 0) == Stereo<float>(-1, -1));
			}

			SUBCASE("side != 0")
			{
				CHECK(ms2stereo<float>(0.5, -0.5) == Stereo<float>(0, 1));
				CHECK(ms2stereo<float>(-0.5, 0.5) == Stereo<float>(0, -1));
				CHECK(ms2stereo<float>(0.5, 0.5) == Stereo<float>(1, 0));
				CHECK(ms2stereo<float>(-0.5, -0.5) == Stereo<float>(-1, 0));
			}
		}
	}
}