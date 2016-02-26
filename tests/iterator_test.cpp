#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>
#include <map>

TEST_CASE("iterator")
{
	cb_tree::cb_set<std::string> cb_set;
	cb_set.insert("a");
	cb_set.insert("ab");
	cb_set.insert("abc");

	std::set<std::string> set;
	set.insert("a");
	set.insert("ab");
	set.insert("abc");

	REQUIRE(cb_set.size() == set.size());
	REQUIRE(std::next(cb_set.begin(), cb_set.size()) == cb_set.end());
	REQUIRE(std::next(cb_set.cbegin(), cb_set.size()) == cb_set.cend());
	REQUIRE(std::next(cb_set.rbegin(), cb_set.size()) == cb_set.rend());
	REQUIRE(std::next(cb_set.crbegin(), cb_set.size()) == cb_set.crend());

	CHECK(std::equal(cb_set.begin(), cb_set.end(), set.begin()));
	CHECK(std::equal(cb_set.cbegin(), cb_set.cend(), set.cbegin()));
	CHECK(std::equal(cb_set.rbegin(), cb_set.rend(), set.rbegin()));
	CHECK(std::equal(cb_set.crbegin(), cb_set.crend(), set.crbegin()));
}
