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

#include <strong_type/istreamable.hpp>

#include "catch2.hpp"

#include <sstream>

TEST_CASE("an istreamable type can be read from a istream")
{
    strong::type<int, struct i_, strong::istreamable> i{0}, j{0};

    std::istringstream is{" 3 4"};
    is >> i >> j;
    REQUIRE(value_of(i) == 3);
    REQUIRE(value_of(j) == 4);
}
