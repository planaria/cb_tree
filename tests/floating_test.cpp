#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>
#include <random>

template <class T>
void test_floating()
{
	std::mt19937 generator;
	std::uniform_real_distribution<T> random_real(
		-std::numeric_limits<T>::max() / 2,
		std::numeric_limits<T>::max() / 2);

	cb_tree::cb_set<T> cb_set;
	std::set<T> set;

	for (std::size_t i = 0; i < 10000; ++i)
	{
		T value = random_real(generator);
		cb_set.insert(value);
		set.insert(value);
	}

	REQUIRE(cb_set.size() == set.size());
	CHECK(std::equal(cb_set.begin(), cb_set.end(), set.begin()));
}

TEST_CASE("floating")
{
	test_floating<float>();
	test_floating<double>();
}
