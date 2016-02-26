#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>

std::size_t count_construction = 0;
std::size_t count_destruction = 0;

struct counter
{

	counter()
	{
		++count_construction;
	}

	~counter()
	{
		++count_destruction;
	}

};

TEST_CASE("erase")
{
	cb_tree::cb_map<std::string, std::unique_ptr<counter>> counters;

	CHECK(count_construction == 0);
	CHECK(count_destruction == 0);

	counters.insert(std::make_pair("a", std::make_unique<counter>()));
	CHECK(counters.size() == 1);

	CHECK(count_construction == 1);
	CHECK(count_destruction == 0);

	counters.insert(std::make_pair("ab", std::make_unique<counter>()));
	CHECK(counters.size() == 2);

	CHECK(count_construction == 2);
	CHECK(count_destruction == 0);

	counters.insert(std::make_pair("abc", std::make_unique<counter>()));
	CHECK(counters.size() == 3);

	CHECK(count_construction == 3);
	CHECK(count_destruction == 0);

	CHECK(counters.erase("") == 0);
	CHECK(counters.erase("abcabc") == 0);
	CHECK(counters.size() == 3);

	CHECK(count_construction == 3);
	CHECK(count_destruction == 0);

	CHECK(counters.erase("ab") == 1);
	CHECK(counters.size() == 2);

	CHECK(count_construction == 3);
	CHECK(count_destruction == 1);

	CHECK(counters.erase("a") == 1);
	CHECK(counters.size() == 1);

	CHECK(count_construction == 3);
	CHECK(count_destruction == 2);

	CHECK(counters.erase("abc") == 1);
	CHECK(counters.size() == 0);

	CHECK(count_construction == 3);
	CHECK(count_destruction == 3);
}
