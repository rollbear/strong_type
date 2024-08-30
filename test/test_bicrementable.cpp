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

#include <strong_type/bicrementable.hpp>

TEST_CASE("a bicrementable goes both ways")
{
    using C = strong::type<int, struct i_, strong::bicrementable>;

    C c{3};
    REQUIRE(value_of(++c) == 4);
    REQUIRE(value_of(c) == 4);
    REQUIRE(value_of(c++) == 4);
    REQUIRE(value_of(c) == 5);
    REQUIRE(value_of(--c) == 4);
    REQUIRE(value_of(c) == 4);
    REQUIRE(value_of(c--) == 4);
    REQUIRE(value_of(c) == 3);
}
