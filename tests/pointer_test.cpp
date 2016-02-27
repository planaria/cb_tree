#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>
#include <random>

template <class T>
void test_pointer()
{
	std::mt19937 generator;
	std::uniform_int_distribution<std::size_t> random_int(
		std::numeric_limits<std::size_t>::min(),
		std::numeric_limits<std::size_t>::max());

	cb_tree::cb_set<T*> cb_set;
	std::set<T*> set;

	for (std::size_t i = 0; i < 10000; ++i)
	{
		T* value = reinterpret_cast<T*>(random_int(generator));
		cb_set.insert(value);
		set.insert(value);
	}

	REQUIRE(cb_set.size() == set.size());
	CHECK(std::equal(cb_set.begin(), cb_set.end(), set.begin()));
}

TEST_CASE("pointer")
{
	test_pointer<void>();
	test_pointer<const void>();
	test_pointer<int>();
	test_pointer<const int>();
}
