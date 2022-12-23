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

#include <strong_type/implicitly_convertible_to.hpp>

#include "catch2.hpp"

#include "test_utils.hpp"

using type = strong::type<int, struct int_, strong::implicitly_convertible_to<int>>;
using multi_type = strong::type<int, struct mint_, strong::implicitly_convertible_to<int, bool, double>>;

TEST_CASE("implicitly_convertible_to can be convert in an explicit construction")
{
    type t{3};
    int x{t};

    REQUIRE(x == 3);
}

TEST_CASE("implicitly_convertible_to can convert in a static_cast")
{
    type t{3};
    int x = 0;
    x = static_cast<int>(t);
    REQUIRE(x == 3);
}

TEST_CASE("implicitly_convertible_to can convert in a comparison")
{
    type t{3};
    REQUIRE(t == 3);
}

template <typename T>
T as(T t) { return t;}
TEST_CASE("implicitly_convertible_to can convert to several types")
{
    multi_type t{3};
    REQUIRE(as<int>(t) == 3);

    REQUIRE(t);

    REQUIRE(as<double>(t) == 3.0);
}

