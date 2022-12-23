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

#include <strong_type/bitarithmetic.hpp>

#include "catch2.hpp"


TEST_CASE("bitarithmetic types can be bitanded")
{
    using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
    using u = uint16_t;

    T i{u(0x0f0f)};
    T j{u(0x5555)};

    REQUIRE(value_of(i & j) == u(0x0505));
    REQUIRE(value_of(i) == u(0x0f0f));
    REQUIRE(value_of(j) == u(0x5555));
    REQUIRE(value_of(i &= j) == u(0x0505));
    REQUIRE(value_of(i) == u(0x0505));
    REQUIRE(value_of(j) == u(0x5555));
}

TEST_CASE("bitarithmetic types can be bitored")
{
    using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
    using u = uint16_t;

    T i{u(0x0f0f)};
    T j{u(0x5555)};

    REQUIRE(value_of(i | j) == u(0x5f5f));
    REQUIRE(value_of(i) == u(0x0f0f));
    REQUIRE(value_of(j) == u(0x5555));
    REQUIRE(value_of(i |= j) == u(0x5f5f));
    REQUIRE(value_of(i) == u(0x5f5f));
    REQUIRE(value_of(j) == u(0x5555));
}

TEST_CASE("bitarithmetic types can be bitxored")
{
    using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
    using u = uint16_t;

    T i{u(0x0f0f)};
    T j{u(0x5555)};

    REQUIRE(value_of(i ^ j) == u(0x5a5a));
    REQUIRE(value_of(i) == u(0x0f0f));
    REQUIRE(value_of(j) == u(0x5555));
    REQUIRE(value_of(i ^= j) == u(0x5a5a));
    REQUIRE(value_of(i) == u(0x5a5a));
    REQUIRE(value_of(j) == u(0x5555));
}

TEST_CASE("bitarithmetic types can be left shifted")
{
    using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
    using u = uint16_t;

    T i{u(0x0f0f)};

    REQUIRE(value_of(i << 2) == u(0x3c3c));
    REQUIRE(value_of(i) == u(0x0f0f));
    REQUIRE(value_of(i <<= 3) == u(0x7878));
    REQUIRE(value_of(i) == u(0x7878));
}

TEST_CASE("bitarithmetic types can be right shifted")
{
    using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
    using u = uint16_t;

    T i{u(0x0f0f)};

    REQUIRE(value_of(i >> 2) == u(0x03c3));
    REQUIRE(value_of(i) == u(0x0f0f));
    REQUIRE(value_of(i >>= 1) == u(0x0787));
    REQUIRE(value_of(i) == u(0x0787));
}

TEST_CASE("bitarithmetic types can be inverted")
{
    using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
    using u = uint16_t;

    T i{u(0x0f0f)};

    REQUIRE(value_of(~i) == u(0xf0f0));
    REQUIRE(value_of(i) == u(0x0f0f));
}
