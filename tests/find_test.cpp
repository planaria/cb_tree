#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>

TEST_CASE("find")
{
	cb_tree::cb_set<std::string> cb_set;

	auto it1 = cb_set.insert("a");
	REQUIRE(it1.second);
	REQUIRE(it1.first != cb_set.end());

	auto it2 = cb_set.insert("ab");
	REQUIRE(it2.second);
	REQUIRE(it2.first != cb_set.end());

	auto it3 = cb_set.insert("abc");
	REQUIRE(it3.second);
	REQUIRE(it3.first != cb_set.end());

	REQUIRE(cb_set.find("a") == it1.first);
	REQUIRE(cb_set.find("ab") == it2.first);
	REQUIRE(cb_set.find("abc") == it3.first);

	CHECK(cb_set.find("") == cb_set.end());
	CHECK(cb_set.find("abcdef") == cb_set.end());
	CHECK(cb_set.find("ac") == cb_set.end());

	CHECK(cb_set.lower_bound("") == it1.first);
	CHECK(cb_set.lower_bound("A") == it1.first);
	CHECK(cb_set.lower_bound("a") == it1.first);
	CHECK(cb_set.lower_bound("aa") == it2.first);
	CHECK(cb_set.lower_bound("ab") == it2.first);
	CHECK(cb_set.lower_bound("ac") == cb_set.end());
	CHECK(cb_set.lower_bound("abb") == it3.first);
	CHECK(cb_set.lower_bound("abc") == it3.first);
	CHECK(cb_set.lower_bound("abd") == cb_set.end());
	CHECK(cb_set.lower_bound("abcdef") == cb_set.end());
	CHECK(cb_set.lower_bound("b") == cb_set.end());

	CHECK(cb_set.upper_bound("") == it1.first);
	CHECK(cb_set.upper_bound("A") == it1.first);
	CHECK(cb_set.upper_bound("a") == it2.first);
	CHECK(cb_set.upper_bound("aa") == it2.first);
	CHECK(cb_set.upper_bound("ab") == it3.first);
	CHECK(cb_set.upper_bound("ac") == cb_set.end());
	CHECK(cb_set.upper_bound("abb") == it3.first);
	CHECK(cb_set.upper_bound("abc") == cb_set.end());
	CHECK(cb_set.upper_bound("abd") == cb_set.end());
	CHECK(cb_set.upper_bound("abcdef") == cb_set.end());
	CHECK(cb_set.upper_bound("b") == cb_set.end());
}

TEST_CASE("find_raw")
{
	cb_tree::cb_set<std::string, cb_tree::cb_raw_string_traits<>> cb_set;

	auto it1 = cb_set.insert("a");
	REQUIRE(it1.second);
	REQUIRE(it1.first != cb_set.end());

	auto it2 = cb_set.insert("ab");
	REQUIRE(it2.second);
	REQUIRE(it2.first != cb_set.end());

	auto it3 = cb_set.insert("abc");
	REQUIRE(it3.second);
	REQUIRE(it3.first != cb_set.end());

	REQUIRE(cb_set.find("a") == it1.first);
	REQUIRE(cb_set.find("ab") == it2.first);
	REQUIRE(cb_set.find("abc") == it3.first);

	CHECK(cb_set.find("") == cb_set.end());
	CHECK(cb_set.find("abcdef") == cb_set.end());
	CHECK(cb_set.find("ac") == cb_set.end());

	CHECK(cb_set.lower_bound("A") == it3.first);
	CHECK(cb_set.lower_bound("aa") == it3.first);
	CHECK(cb_set.lower_bound("abcdef") == it3.first);
	CHECK(cb_set.lower_bound("abb") == it3.first);
	CHECK(cb_set.lower_bound("abc") == it3.first);
	CHECK(cb_set.lower_bound("abd") == it2.first);
	CHECK(cb_set.lower_bound("ab") == it2.first);
	CHECK(cb_set.lower_bound("ac") == it1.first);
	CHECK(cb_set.lower_bound("a") == it1.first);
	CHECK(cb_set.lower_bound("b") == cb_set.end());
	CHECK(cb_set.lower_bound("") == cb_set.end());

	CHECK(cb_set.upper_bound("A") == it3.first);
	CHECK(cb_set.upper_bound("aa") == it3.first);
	CHECK(cb_set.upper_bound("abcdef") == it3.first);
	CHECK(cb_set.upper_bound("abb") == it3.first);
	CHECK(cb_set.upper_bound("abc") == it2.first);
	CHECK(cb_set.upper_bound("abd") == it2.first);
	CHECK(cb_set.upper_bound("ab") == it1.first);
	CHECK(cb_set.upper_bound("ac") == it1.first);
	CHECK(cb_set.upper_bound("a") == cb_set.end());
	CHECK(cb_set.upper_bound("b") == cb_set.end());
	CHECK(cb_set.upper_bound("") == cb_set.end());
}
