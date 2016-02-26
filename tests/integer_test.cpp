#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>
#include <random>

template <class T>
void test_integer()
{
	std::mt19937 generator;
	std::uniform_int_distribution<T> random_int(
		std::numeric_limits<T>::min(),
		std::numeric_limits<T>::max());

	cb_tree::cb_set<T> cb_set;
	std::set<T> set;

	for (std::size_t i = 0; i < 10000; ++i)
	{
		T value = random_int(generator);
		cb_set.insert(value);
		set.insert(value);
	}

	REQUIRE(cb_set.size() == set.size());
	CHECK(std::equal(cb_set.begin(), cb_set.end(), set.begin()));
}

TEST_CASE("integer")
{
	test_integer<std::uint16_t>();
	test_integer<std::uint32_t>();
	test_integer<std::uint64_t>();
	test_integer<std::int16_t>();
	test_integer<std::int32_t>();
	test_integer<std::int64_t>();
}
