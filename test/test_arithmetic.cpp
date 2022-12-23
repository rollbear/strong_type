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

#include <strong_type/arithmetic.hpp>

#include "catch2.hpp"

namespace strong
{
TEST_CASE("arithmetic types can be added")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{3};
    T j{4};
    REQUIRE(value_of(i + j) == 7);
    REQUIRE(value_of(i) == 3);
    REQUIRE(value_of(j) == 4);
    i += j;
    REQUIRE(value_of(i) == 7);
    REQUIRE(value_of(j) == 4);
}

TEST_CASE("arithmetic types can be subtracted")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{7};
    T j{4};
    REQUIRE(value_of(i - j) == 3);
    REQUIRE(value_of(i) == 7);
    REQUIRE(value_of(j) == 4);
    i -= j;
    REQUIRE(value_of(i) == 3);
    REQUIRE(value_of(j) == 4);
}

TEST_CASE("arithmetic types can be multiplied")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{12};
    T j{3};
    REQUIRE(value_of(i * j) == 36);
    REQUIRE(value_of(i) == 12);
    REQUIRE(value_of(j) == 3);
    i *= j;
    REQUIRE(value_of(i) == 36);
    REQUIRE(value_of(j) == 3);
}

TEST_CASE("arithmetic types can be divided")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{12};
    T j{3};
    REQUIRE(value_of(i / j) == 4);
    REQUIRE(value_of(i) == 12);
    REQUIRE(value_of(j) == 3);
    i /= j;
    REQUIRE(value_of(i) == 4);
    REQUIRE(value_of(j) == 3);
}

TEST_CASE("the remainder of integral arithmetic types can be calculated")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{12};
    T j{5};
    REQUIRE(value_of(i % j) == 2);
    REQUIRE(value_of(i) == 12);
    REQUIRE(value_of(j) == 5);
    i %= j;
    REQUIRE(value_of(i) == 2);
    REQUIRE(value_of(j) == 5);
}

TEST_CASE("an aritmmetic type can be negated")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{12};
    REQUIRE(value_of(-i) == -12);
    REQUIRE(value_of(i) == 12);
}

}