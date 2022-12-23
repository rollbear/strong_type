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

#include <strong_type/equality_with.hpp>

#include "catch2.hpp"

TEST_CASE("equality_with can compare with defined type")
{
    using t1 = strong::type<int, struct t1_>;
    using t2 = strong::type<int, struct t2_, strong::equality_with<t1>>;

    REQUIRE(t1{1} == t2{1});
    REQUIRE(t2{1} == t1{1});
    REQUIRE(t1{1} != t2{2});
    REQUIRE(t2{2} != t1{1});
}
