#include "doctest.h"

#include "../CircularBuffer.hpp"

using namespace dsp;

TEST_CASE("CircularBuffer")
{
	SUBCASE("Empty initialization")
	{
		CircularBuffer<int> buffer(5);

		CHECK(buffer.size() == 5);
		for (auto i = 0; i < 5; ++i)
			CHECK(buffer[i] == 0);
	}

	SUBCASE("Initializer list")
	{
		CircularBuffer<int> buffer = { 4, 4, 4, 4 };

		CHECK(buffer.size() == 4);
		for (auto i = 0; i < 4; ++i)
			CHECK(buffer[i] == 4);
	}

	SUBCASE("emplace_back")
	{
		CircularBuffer<int> buffer(3);	
		buffer.emplace_back(8);
		buffer.emplace_back(9);

		REQUIRE(buffer.size() == 3);
		CHECK(buffer[buffer.size() - 2] == 8);
		CHECK(buffer[buffer.size() - 1] == 9);
	}

	SUBCASE("Subscript")
	{
		CircularBuffer<int> buffer(4);

		for (auto i = 0; i < 4; ++i)
		{
			REQUIRE_NOTHROW(buffer[0]);
			buffer[0] = i * 2;
			CHECK(buffer[0] == i + i);
		}

		SUBCASE("Out of Range")
		{
			CHECK_THROWS_AS(buffer[4], std::out_of_range);
			CHECK_THROWS_AS(buffer[15], std::out_of_range);
			CHECK_THROWS_AS(buffer[-1], std::out_of_range);
		}
	}

	SUBCASE("Iterators")
	{
		CircularBuffer<int> buffer = { 0, 1, 2, 3, 4 };

		SUBCASE("regular")
		{
			int i = 0;
			for (auto it = buffer.begin(); it != buffer.end(); ++it)
				CHECK(*it == i++);
		}

		SUBCASE("regular const")
		{
			int i = 0;
			for (auto it = buffer.cbegin(); it != buffer.cend(); ++it)
				CHECK(*it == i++);
		}

		SUBCASE("reverse")
		{
			int i = 4;
			for (auto it = buffer.rbegin(); it != buffer.rend(); ++it)
				CHECK(*it == i--);
		}

		SUBCASE("reverse const")
		{
			int i = 4;
			for (auto it = buffer.crbegin(); it != buffer.crend(); ++it)
				CHECK(*it == i--);
		}
	}
}