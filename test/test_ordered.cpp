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

#include <strong_type/ordered.hpp>

#include "catch2.hpp"

TEST_CASE("ordered type can be compared for ordering")
{
    using type = strong::type<int, struct i_, strong::ordered>;

    const type i1{1};
    const type i2{2};

    REQUIRE(i1 < i2);
    REQUIRE(i1 <= i2);
    REQUIRE(i1 <= i1);
    REQUIRE(i2 > i1);
    REQUIRE(i2 >= i2);
    REQUIRE(i2 >= i1);

    REQUIRE_FALSE(i2 < i1);
    REQUIRE_FALSE(i2 <= i1);
    REQUIRE_FALSE(i1 > i2);
    REQUIRE_FALSE(i1 >= i2);
}
