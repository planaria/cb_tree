#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>

TEST_CASE("construct")
{
	cb_tree::cb_set<std::string> org;
	CHECK(org.empty());
	CHECK(org.size() == 0);
}

TEST_CASE("copy_move_construct")
{
	cb_tree::cb_set<std::string> org =
	{
		"a",
		"ab",
		"abc",
	};

	auto copy = org;
	REQUIRE(copy == org);

	auto move = std::move(copy);
	CHECK(move == org);
}
