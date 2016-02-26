#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>
#include <random>
#include "test_util.hpp"

TEST_CASE("insert")
{
	std::mt19937 generator;
	std::uniform_int_distribution<int> random_char(std::numeric_limits<char>::min(), std::numeric_limits<char>::max());
	std::uniform_real_distribution<> random_real(0.0, 1.0);

	auto generate_random_string = [&]()
	{
		std::string result;

		while (random_real(generator) < 0.9)
			result += static_cast<char>(random_char(generator));

		result = encode_base64(result);

		return result;
	};

	for (std::size_t i = 0; i < 100; ++i)
	{
		cb_tree::cb_set<std::string> cb_set;
		std::set<std::string> set;

		for (std::size_t j = 0; j < 100; ++j)
		{
			std::string str = generate_random_string();
			cb_set.insert(str);
			set.insert(str);
		}

		REQUIRE(cb_set.size() == set.size());
		CHECK(std::equal(cb_set.begin(), cb_set.end(), set.begin()));
	}
}
