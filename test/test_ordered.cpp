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

#include <strong_type/ordered.hpp>

#include "catch2.hpp"

#include <cmath>

TEST_CASE("ordered type can be compared for ordering")
{
    using type = strong::type<int, struct i_, strong::ordered>;

    const type i1{1};
    const type i2{2};

    REQUIRE(i1 < i2);
    REQUIRE(i1 <= i2);
    REQUIRE(i1 <= i1);
    REQUIRE(i2 > i1);
    REQUIRE(i2 >= i2);
    REQUIRE(i2 >= i1);

    REQUIRE_FALSE(i2 < i1);
    REQUIRE_FALSE(i2 <= i1);
    REQUIRE_FALSE(i1 > i2);
    REQUIRE_FALSE(i1 >= i2);
}

#if __cpp_impl_three_way_comparison && __has_include(<compare>) && (!defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 15000)

namespace {
    template<typename T, typename = void>
    struct three_way_comparable : std::false_type {
    };

    template<typename T>
    struct three_way_comparable<T, std::void_t<decltype(std::declval<T>() <=> std::declval<T>())>> : std::true_type {
    };

    template<typename T>
    static constexpr bool three_way_comparable_v = three_way_comparable<T>::value;
}
using istrong = strong::type<int, struct i_, strong::strongly_ordered>;
using iweak = strong::type<int, struct i_, strong::weakly_ordered>;
using ipartial = strong::type<int, struct i_, strong::partially_ordered>;
using dstrong = strong::type<double, struct i_, strong::strongly_ordered>;
using dweak = strong::type<double, struct i_, strong::weakly_ordered>;
using dpartial = strong::type<double, struct i_, strong::partially_ordered>;
static_assert(three_way_comparable_v<istrong>);
static_assert(three_way_comparable_v<iweak>);
static_assert(three_way_comparable_v<ipartial>);
static_assert(!three_way_comparable_v<dstrong>);
static_assert(!three_way_comparable_v<dweak>);
static_assert(three_way_comparable_v<dpartial>);
static_assert(std::is_same_v<std::compare_three_way_result_t<istrong>, std::strong_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<iweak>, std::weak_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<ipartial>, std::partial_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<dpartial>, std::partial_ordering>);

static_assert(istrong{1} < istrong{2});
static_assert(iweak{1} < iweak{2});
static_assert(ipartial{1} < ipartial{2});
static_assert(dpartial{1} < dpartial{2});

TEST_CASE("strongly_ordered type can be compared for ordering")
{
    using type = strong::type<int, struct i_, strong::strongly_ordered>;

    const type i1{1};
    const type i2{2};

    static_assert(std::is_same_v<decltype(i1 <=> i2), std::strong_ordering>);

    REQUIRE(i1 < i2);
    REQUIRE(i1 <= i2);
    REQUIRE(i1 <= i1);
    REQUIRE(i2 > i1);
    REQUIRE(i2 >= i2);
    REQUIRE(i2 >= i1);

    REQUIRE_FALSE(i2 < i1);
    REQUIRE_FALSE(i2 <= i1);
    REQUIRE_FALSE(i1 > i2);
    REQUIRE_FALSE(i1 >= i2);

    REQUIRE((i1 <=> i2) == std::strong_ordering::less);
}

TEST_CASE("weakly_ordered type can be compared for ordering")
{
    using type = strong::type<int, struct i_, strong::weakly_ordered>;

    const type i1{1};
    const type i2{2};

    static_assert(std::is_same_v<decltype(i1 <=> i2), std::weak_ordering>);

    REQUIRE(i1 < i2);
    REQUIRE(i1 <= i2);
    REQUIRE(i1 <= i1);
    REQUIRE(i2 > i1);
    REQUIRE(i2 >= i2);
    REQUIRE(i2 >= i1);

    REQUIRE_FALSE(i2 < i1);
    REQUIRE_FALSE(i2 <= i1);
    REQUIRE_FALSE(i1 > i2);
    REQUIRE_FALSE(i1 >= i2);

    REQUIRE((i1 <=> i2) == std::weak_ordering::less);
}

TEST_CASE("partially_ordered type can be compared for ordering")
{
    using type = strong::type<double, struct i_, strong::partially_ordered>;

    const type i1{1};
    const type i2{2};

    static_assert(std::is_same_v<decltype(i1 <=> i2), std::partial_ordering>);

    REQUIRE(i1 < i2);
    REQUIRE(i1 <= i2);
    REQUIRE(i1 <= i1);
    REQUIRE(i2 > i1);
    REQUIRE(i2 >= i2);
    REQUIRE(i2 >= i1);

    REQUIRE_FALSE(i2 < i1);
    REQUIRE_FALSE(i2 <= i1);
    REQUIRE_FALSE(i1 > i2);
    REQUIRE_FALSE(i1 >= i2);

    REQUIRE((i1 <=> i2) == std::partial_ordering::less);

    REQUIRE((i1 <=> type{NAN}) == std::partial_ordering::unordered);
}

#endif
