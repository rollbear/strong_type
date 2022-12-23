/*
 * strong_type C++14/17/20 strong typedef library
 *
 * Copyright (C) Björn Fahller
 *
 *  Use, modification and distribution is subject to the
 *  Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 * Project home: https://github.com/rollbear/strong_type
 */

#include <strong_type/ordered_with.hpp>

#include "catch2.hpp"

TEST_CASE("ordered_with")
{
    const strong::type<int, struct int_, strong::ordered_with<int>> i1{1};

    REQUIRE(i1 < 2);
    REQUIRE(0 < i1);
    REQUIRE_FALSE(i1 < 1);
    REQUIRE_FALSE(1 < i1);
    REQUIRE(i1 <= 1);
    REQUIRE(i1 <= 2);
    REQUIRE_FALSE(i1 <= 0);
    REQUIRE(1 <= i1);
    REQUIRE(0 <= i1);
    REQUIRE_FALSE(2<= i1);
    REQUIRE(i1 >= 1);
    REQUIRE(i1 >= 0);
    REQUIRE_FALSE(i1 >= 2);
    REQUIRE(1 >= i1);
    REQUIRE(2 >= i1);
    REQUIRE_FALSE(0 >= i1);
    REQUIRE(i1 > 0);
    REQUIRE_FALSE(i1 > 1);
    REQUIRE(2 > i1);
    REQUIRE_FALSE(1 > i1);
}
