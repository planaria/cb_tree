#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>

TEST_CASE("copy_assignment")
{
	cb_tree::cb_set<std::string> org =
	{
		"a",
		"ab",
		"abc",
	};

	cb_tree::cb_set<std::string> copy;
	copy = org;
	CHECK(copy == org);
}

TEST_CASE("move_assignment")
{
	cb_tree::cb_set<std::string> org =
	{
		"a",
		"ab",
		"abc",
	};

	cb_tree::cb_set<std::string> copy;
	copy = org;
	REQUIRE(copy == org);

	cb_tree::cb_set<std::string> move;
	move = std::move(copy);
	CHECK(move == org);
}
