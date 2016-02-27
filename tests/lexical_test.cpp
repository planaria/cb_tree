#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>

TEST_CASE("longest_match")
{
	cb_tree::cb_set<std::string> cb_set;

	auto it1 = cb_set.insert("a");
	auto it2 = cb_set.insert("ab");
	auto it3 = cb_set.insert("abc");
	auto it4 = cb_set.insert("abcdef");
	auto it5 = cb_set.insert("c");

	CHECK(cb_set.longest_match("") == std::make_pair(it1.first, cb_set.end()));
	CHECK(cb_set.longest_match("A") == std::make_pair(it1.first, cb_set.end()));
	CHECK(cb_set.longest_match("a") == std::make_pair(it1.first, it5.first));
	CHECK(cb_set.longest_match("aa") == std::make_pair(it1.first, it5.first));
	CHECK(cb_set.longest_match("ab") == std::make_pair(it2.first, it5.first));
	CHECK(cb_set.longest_match("ac") == std::make_pair(it1.first, it5.first));
	CHECK(cb_set.longest_match("abc") == std::make_pair(it3.first, it5.first));
	CHECK(cb_set.longest_match("abcd") == std::make_pair(it4.first, it5.first));
	CHECK(cb_set.longest_match("b") == std::make_pair(it1.first, cb_set.end()));

	CHECK(cb_set.prefix_match("") == std::make_pair(it1.first, cb_set.end()));
	CHECK(cb_set.prefix_match("A") == std::make_pair(cb_set.end(), cb_set.end()));
	CHECK(cb_set.prefix_match("a") == std::make_pair(it1.first, it5.first));
	CHECK(cb_set.prefix_match("aa") == std::make_pair(cb_set.end(), cb_set.end()));
	CHECK(cb_set.prefix_match("ab") == std::make_pair(it2.first, it5.first));
	CHECK(cb_set.prefix_match("ac") == std::make_pair(cb_set.end(), cb_set.end()));
	CHECK(cb_set.prefix_match("abc") == std::make_pair(it3.first, it5.first));
	CHECK(cb_set.prefix_match("abcd") == std::make_pair(it4.first, it5.first));
	CHECK(cb_set.prefix_match("b") == std::make_pair(cb_set.end(), cb_set.end()));
}
