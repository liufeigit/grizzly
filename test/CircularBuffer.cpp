#include <vector>

#include "doctest.h"

#include "../CircularBuffer.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("CircularBuffer")
{
	SUBCASE("Construct empty initialization")
	{
		CircularBuffer<int> buffer(5);

		CHECK(buffer.size() == 5);
		for (auto i = 0; i < 5; ++i)
			CHECK(buffer[i] == 0);

		SUBCASE("size 0")
		{
			CircularBuffer<int> buffer(0);
			CHECK(buffer.size() == 0);
		}
	}

	SUBCASE("Construct with initializer list")
	{
		CircularBuffer<int> buffer = { 4, 4, 4, 4 };

		CHECK(buffer.size() == 4);
		for (auto i = 0; i < 4; ++i)
			CHECK(buffer[i] == 4);
	}

	SUBCASE("Construct with iterators")
	{
		vector<int> vec = { 1, 1, 1, 1, 1, 1 };
		CircularBuffer<int> buffer(vec.begin(), vec.end());

		CHECK(buffer.size() == 6);
		for (auto i = 0; i < 6; ++i)
			CHECK(buffer[i] == 1);
	}

	SUBCASE("emplace_back")
	{
		CircularBuffer<int> buffer(3);	
		buffer.emplace_back(8);
		buffer.emplace_back(9);

		REQUIRE(buffer.size() == 3);
		CHECK(buffer[buffer.size() - 2] == 8);
		CHECK(buffer[buffer.size() - 1] == 9);

		buffer.emplace_back(1);
		buffer.emplace_back(13);
		buffer.emplace_back(-8);

		CHECK(buffer[buffer.size() - 3] == 1);
		CHECK(buffer[buffer.size() - 2] == 13);
		CHECK(buffer[buffer.size() - 1] == -8);
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

		SUBCASE("size 0")
		{
			CircularBuffer<int> buffer(0);
			CHECK(buffer.begin() == buffer.end());
			CHECK(buffer.cbegin() == buffer.cend());
			CHECK(buffer.rbegin() == buffer.rend());
			CHECK(buffer.crbegin() == buffer.crend());
		}
	}
}