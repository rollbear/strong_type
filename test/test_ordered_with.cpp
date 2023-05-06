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

#include <strong_type/ordered_with.hpp>

#include "catch2.hpp"

#include <cmath>

TEST_CASE("ordered_with")
{
    const strong::type<int, struct int_, strong::ordered_with<int>> i1{1};

    REQUIRE(i1 < 2);
    REQUIRE(0 < i1);
    REQUIRE_FALSE(i1 < 1);
    REQUIRE_FALSE(1 < i1);
    REQUIRE(i1 <= 1);
    REQUIRE(i1 <= 2);
    REQUIRE_FALSE(i1 <= 0);
    REQUIRE(1 <= i1);
    REQUIRE(0 <= i1);
    REQUIRE_FALSE(2<= i1);
    REQUIRE(i1 >= 1);
    REQUIRE(i1 >= 0);
    REQUIRE_FALSE(i1 >= 2);
    REQUIRE(1 >= i1);
    REQUIRE(2 >= i1);
    REQUIRE_FALSE(0 >= i1);
    REQUIRE(i1 > 0);
    REQUIRE_FALSE(i1 > 1);
    REQUIRE(2 > i1);
    REQUIRE_FALSE(1 > i1);
}

#if __cpp_impl_three_way_comparison && __has_include(<compare>) && (!defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 15000)
namespace {
template<typename T, typename U, typename = void>
struct three_way_comparable : std::false_type {
};

template<typename T, typename U>
struct three_way_comparable<T, U, std::void_t<decltype(std::declval<T>() <=> std::declval<U>())>> : std::true_type {
};

template<typename T, typename U>
static constexpr bool three_way_comparable_v = three_way_comparable<T, U>::value;
}

using ihandle = strong::type<int, struct ihandle_>;
using dhandle = strong::type<double, struct dhandle_>;
using istrong = strong::type<int, struct i_, strong::strongly_ordered_with<int, ihandle>>;
using iweak = strong::type<int, struct i_, strong::weakly_ordered_with<int, ihandle>>;
using ipartial = strong::type<int, struct i_, strong::partially_ordered_with<int, ihandle>>;
using dstrong = strong::type<double, struct d_, strong::strongly_ordered_with<int, ihandle>>;
using dweak = strong::type<double, struct d_, strong::weakly_ordered_with<int, ihandle>>;
using dpartial = strong::type<double, struct d_, strong::partially_ordered_with<int, ihandle>>;

static_assert(!three_way_comparable_v<ihandle, ihandle>);

static_assert(three_way_comparable_v<ihandle, istrong>);
static_assert(three_way_comparable_v<int, istrong>);
static_assert(!three_way_comparable_v<dhandle, istrong>);
static_assert(three_way_comparable_v<ihandle, iweak>);
static_assert(three_way_comparable_v<int, iweak>);
static_assert(!three_way_comparable_v<dhandle, iweak>);
static_assert(three_way_comparable_v<ihandle, ipartial>);
static_assert(three_way_comparable_v<int, ipartial>);
static_assert(!three_way_comparable_v<dhandle, ipartial>);

static_assert(!three_way_comparable_v<ihandle,dstrong>);
static_assert(!three_way_comparable_v<int, dstrong>);
static_assert(!three_way_comparable_v<dhandle, dstrong>);
static_assert(!three_way_comparable_v<ihandle, dweak>);
static_assert(!three_way_comparable_v<int, dweak>);
static_assert(!three_way_comparable_v<dhandle, dweak>);
static_assert(three_way_comparable_v<ihandle, dpartial>);
static_assert(three_way_comparable_v<int, dpartial>);
static_assert(!three_way_comparable_v<dhandle, dpartial>);

static_assert(std::is_same_v<std::compare_three_way_result_t<istrong, int>, std::strong_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<istrong, ihandle>, std::strong_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<iweak, int>, std::weak_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<iweak, ihandle>, std::weak_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<ipartial, int>, std::partial_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<ipartial, ihandle>, std::partial_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<dpartial, int>, std::partial_ordering>);
static_assert(std::is_same_v<std::compare_three_way_result_t<dpartial, ihandle>, std::partial_ordering>);

static_assert(istrong{1} < 2);
static_assert(istrong{1} < ihandle{2});
static_assert(iweak{1} < 2);
static_assert(iweak{1} < ihandle{2});
static_assert(ipartial{1} < 2);
static_assert(ipartial{1} < ihandle{2});
static_assert(dpartial{1} < 2);
static_assert(dpartial{1} < ihandle{2});

TEST_CASE("strongly_ordered_with types can be compared for ordering")
{
    using type = strong::type<int, struct i_, strong::strongly_ordered_with<int, ihandle>>;

    const type i1{1};

    static_assert(std::is_same_v<decltype(i1 <=> 2), std::strong_ordering>);
    static_assert(std::is_same_v<decltype(i1 <=> ihandle{2}), std::strong_ordering>);

    REQUIRE(i1 < 2);
    REQUIRE(i1 <= 2);
    REQUIRE(i1 <= 1);
    REQUIRE(2 > i1);
    REQUIRE(1 >= i1);
    REQUIRE(2 >= i1);

    REQUIRE_FALSE(2 < i1);
    REQUIRE_FALSE(2 <= i1);
    REQUIRE_FALSE(i1 > 2);
    REQUIRE_FALSE(i1 >= 2);

    REQUIRE((i1 <=> 2) == std::strong_ordering::less);
    REQUIRE((i1 <=> ihandle{2}) == std::strong_ordering::less);
}

TEST_CASE("weakly_ordered_with types can be compared for ordering")
{
    using type = strong::type<int, struct i_, strong::weakly_ordered_with<int, ihandle>>;

    const type i1{1};

    static_assert(std::is_same_v<decltype(i1 <=> 2), std::weak_ordering>);
    static_assert(std::is_same_v<decltype(i1 <=> ihandle{2}), std::weak_ordering>);

    REQUIRE(i1 < 2);
    REQUIRE(i1 <= 2);
    REQUIRE(i1 <= 1);
    REQUIRE(2 > i1);
    REQUIRE(1 >= i1);
    REQUIRE(2 >= i1);

    REQUIRE_FALSE(2 < i1);
    REQUIRE_FALSE(2 <= i1);
    REQUIRE_FALSE(i1 > 2);
    REQUIRE_FALSE(i1 >= 2);

    REQUIRE((i1 <=> 2) == std::weak_ordering::less);
    REQUIRE((i1 <=> ihandle{2}) == std::weak_ordering::less);
}

TEST_CASE("partially_ordered_with types can be compared for ordering")
{
    using type = strong::type<double, struct i_, strong::partially_ordered_with<int, ihandle>>;

    const type i1{1};

    static_assert(std::is_same_v<decltype(i1 <=> 2), std::partial_ordering>);
    static_assert(std::is_same_v<decltype(i1 <=> ihandle{2}), std::partial_ordering>);

    REQUIRE(i1 < 2);
    REQUIRE(i1 <= 2);
    REQUIRE(i1 <= 1);
    REQUIRE(2 > i1);
    REQUIRE(1 >= i1);
    REQUIRE(2 >= i1);

    REQUIRE_FALSE(2 < i1);
    REQUIRE_FALSE(2 <= i1);
    REQUIRE_FALSE(i1 > 2);
    REQUIRE_FALSE(i1 >= 2);

    REQUIRE((i1 <=> 2) == std::partial_ordering::less);
    REQUIRE((i1 <=> ihandle{2}) == std::partial_ordering::less);

    REQUIRE((1 <=> type{NAN}) == std::partial_ordering::unordered);
}

#endif