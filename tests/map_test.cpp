#include <catch.hpp>
#include <cb_tree/cb_tree.hpp>

TEST_CASE("map")
{
	cb_tree::cb_map<std::string, int> cb_map;

	cb_map.insert(std::make_pair("abc", 1));
	cb_map["def"] = 2;

	CHECK(cb_map.at("abc") == 1);
	CHECK(cb_map.at("def") == 2);
	CHECK_THROWS_AS(cb_map.at("ghi"), std::out_of_range);
}
