//
// Created by fatih on 6/4/17.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <malt/malt_reflect.hpp>

struct foo
{
    int x;
    float y;

    REFLECT(foo, MEM(x), MEM(y))
};


TEST_CASE("static reflection test", "[malt_core]")
{
    auto res = foo::reflect();
    REQUIRE(std::string(res.name) == "foo");

    foo v;
    malt::meta::apply(res.members, [&](auto& var){
        v.*(var.member) = 3;
    });

    REQUIRE(v.x == 3);
    REQUIRE(v.y == 3);
}