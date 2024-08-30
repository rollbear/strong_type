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

#include <strong_type/affine_point.hpp>


TEST_CASE("affine_point types can be subtracted")
{
    using D = strong::type<int, struct i_>;
    using T = strong::type<int, struct i_, strong::affine_point<D>>;

    T t1{3};
    T t2{8};
    auto d = t2 - t1;
    static_assert(std::is_same<decltype(d), D>{}, "");
    REQUIRE(value_of(d) == 5);
}

TEST_CASE("affine point type with defaulted difference type yields it when subtracting")
{
    using T = strong::type<int, struct i_, strong::affine_point<>>;

    T t1{3};
    T t2{8};
    auto d = t2 - t1;
    static_assert(std::is_same<decltype(d), strong::type<int, struct i_, strong::difference>>{},"");
    REQUIRE(value_of(d) == 5);
}

TEST_CASE("affine_point types can be added with the delta type")
{
    using D = strong::type<int, struct i_>;
    using T = strong::type<int, struct i_, strong::affine_point<D>>;

    T t1{8};
    D d{3};

    auto t2 = t1 + d;
    static_assert(std::is_same<decltype(t2), T>{}, "");
    REQUIRE(value_of(t2) == 11);
    auto t3 = d + t1;
    static_assert(std::is_same<decltype(t3), T>{}, "");
    REQUIRE(value_of(t3) == 11);
    t1 += d;
    REQUIRE(value_of(t1) == 11);
}

TEST_CASE("affine_point types can be subtracted with the delta type")
{
    using D = strong::type<int, struct i_>;
    using T = strong::type<int, struct i_, strong::affine_point<D>>;

    T t1{8};
    D d{3};

    auto t2 = t1 - d;
    static_assert(std::is_same<decltype(t2), T>{}, "");
    REQUIRE(value_of(t2) == 5);
    t1 -= d;
    REQUIRE(value_of(t1) == 5);
}
