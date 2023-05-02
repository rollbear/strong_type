/*
 * strong_type C++14/17/20 strong typedef library
 *
 * Copyright (C) Björn Schäpers
 *
 *  Use, modification and distribution is subject to the
 *  Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 * Project home: https://github.com/rollbear/strong_type
 */

#include <strong_type/three_way.hpp>

#if STRONG_HAS_COMPARE
#include "catch2.hpp"

TEST_CASE("values can be compared using operator== through operator<=>")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE_FALSE(i == j);
    REQUIRE(i == k);
}

TEST_CASE("values can be compared using operator!= through operator<=>")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE(i != j);
    REQUIRE_FALSE(i != k);
}

TEST_CASE("values can be compared using operator< through operator<=>")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE(i < j);
    REQUIRE_FALSE(i < k);
}

TEST_CASE("values can be compared using operator<= through operator<=>")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE(i <= j);
    REQUIRE(i <= k);
    REQUIRE_FALSE(j <= i);
}

TEST_CASE("values can be compared using operator> through operator<=>")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE_FALSE(i > j);
    REQUIRE_FALSE(i > k);
    REQUIRE(j > i);
}

TEST_CASE("values can be compared using operator>= through operator<=>")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    type i{3};
    type j{4};
    type k{3};
    REQUIRE_FALSE(i >= j);
    REQUIRE(i >= k);
    REQUIRE(j >= i);
}

TEST_CASE("can use defaulted three way operator in structs")
{
    using type = strong::type<int, struct i_, strong::three_way>;

    struct s
    {
        type t;
        constexpr auto operator<=>(const s&) const noexcept = default;
    };

    s i{type{3}};
    s j{type{4}};
    s k{type{3}};
    REQUIRE(i == k);
    REQUIRE(i < j);
    REQUIRE(i <= j);
    REQUIRE(j > k);
    REQUIRE(j >= k);

    REQUIRE_FALSE(i == j);
    REQUIRE_FALSE(j < i);
    REQUIRE_FALSE(j <= i);
    REQUIRE_FALSE(i > j);
    REQUIRE_FALSE(i >= j);
}
#endif //STRONG_HAS_COMPARE
