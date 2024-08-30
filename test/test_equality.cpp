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

 #include "catch2.hpp"

#include <strong_type/equality.hpp>

TEST_CASE("values can be compared using operator==")
{
    using type = strong::type<int, struct i_, strong::equality>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE_FALSE(i == j);
    REQUIRE(i == k);
}

TEST_CASE("values can be compared using operator!=")
{
    using type = strong::type<int, struct i_, strong::equality>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE(i != j);
    REQUIRE(!(i != k));
}
