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

#include <strong_type/decrementable.hpp>

#include "catch2.hpp"

TEST_CASE("a decrementable can be decremented")
{
    using C = strong::type<int, struct i_, strong::decrementable>;

    C c{10};
    REQUIRE(value_of(--c) == 9);
    REQUIRE(value_of(c) == 9);
    REQUIRE(value_of(c--) == 9);
    REQUIRE(value_of(c) == 8);
}
