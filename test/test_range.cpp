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

#include <strong_type/range.hpp>

#include "catch2.hpp"

TEST_CASE("a range can be used with range based for")
{
    using iv = strong::type<std::vector<int>, struct vi_, strong::range>;

    iv v{3,2,1};
    int n = 3;
    for (auto& e : v)
    {
        REQUIRE(e-- == n--);
    }
    auto& cv = v;
    n = 2;
    for (auto& e : cv)
    {
        REQUIRE(e == n--);
    }
}

TEST_CASE("iterator type can be used from range")
{
    using iv = strong::type<std::vector<int>, struct vi_, strong::range>;

    iv v{3,2,1};
    int n = 3;
    for (iv::iterator i{v.begin()}; i != v.end(); ++i)
    {
        REQUIRE((*i)-- == n--);
    }
    const auto& cv = v;
    n = 2;
    for (iv::const_iterator i{cv.begin()}; i != cv.end(); ++i)
    {
        REQUIRE(*i == n--);
    }
    n = 2;
    for (iv::const_iterator i{v.cbegin()}; i != v.cend(); ++i)
    {
        REQUIRE(*i == n--);
    }
}
