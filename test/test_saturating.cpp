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

#include <strong_type/saturating_arithmetic.hpp>

#ifdef __cpp_lib_saturation_arithmetic

#include <catch2/catch_template_test_macros.hpp>

#include "strong_type/equality.hpp"


TEMPLATE_TEST_CASE("saturating + limits the result to max/min for the underlying type", "[saturating]", int, short, long, signed char, unsigned char, unsigned long)
{
    using type = strong::type<TestType, struct tag, strong::saturating_arithmetic>;
    const auto max = type{std::numeric_limits<TestType>::max()};
    const auto min = type{std::numeric_limits<TestType>::min()};
    REQUIRE(value_of(max + type{1}) == value_of(max));
    REQUIRE(value_of(max + max) == value_of(max));
    REQUIRE(value_of(min + min) == value_of(min));
    if constexpr (std::is_signed_v<TestType>)
    {
        REQUIRE(value_of(min + type{-1}) == value_of(min));
    }
}

TEMPLATE_TEST_CASE("saturating - limits the result to max/min for the underlying type","[saturating]", int, short, long, signed char, unsigned char, unsigned long)
{
    using type = strong::type<TestType, struct tag, strong::saturating_arithmetic>;
    const auto max = type{std::numeric_limits<TestType>::max()};
    const auto min = type{std::numeric_limits<TestType>::min()};
    REQUIRE(value_of(min - type{1}) == value_of(min));
    REQUIRE(value_of(min - max) == value_of(min));
    REQUIRE(value_of(max - min) == value_of(max));
    if constexpr (std::is_signed_v<TestType>)
    {
        REQUIRE(value_of(max - type{-1}) == value_of(max));
    }
}

TEMPLATE_TEST_CASE("saturating * limits the result to min/max for the underlying type", "[saturating]", int, short, long, signed char, unsigned char, unsigned long)
{
    using type = strong::type<TestType, struct tag, strong::saturating_arithmetic>;
    const auto max = type{std::numeric_limits<TestType>::max()};
    const auto min = type{std::numeric_limits<TestType>::min()};
    REQUIRE(value_of(max / type{2} * type{3}) == value_of(max));
    REQUIRE(value_of(min / type{2} * type{3}) == value_of(min));
    REQUIRE(value_of(max / type{3} * type{2}) == value_of(max)/3*2);
    REQUIRE(value_of(min / type{3} * type{2}) == value_of(min)/3*2);
}

TEMPLATE_TEST_CASE("saturating / makes min/-1 == max for signed numbers", "[saturating]", int, short, signed char, long)
{
    using type = strong::type<TestType, struct tag, strong::saturating_arithmetic, strong::equality>;
    const auto max = type{std::numeric_limits<TestType>::max()};
    const auto min = type{std::numeric_limits<TestType>::min()};

    REQUIRE(min / type{-1} == max);
}

TEST_CASE("construction from larger type saturates value", "[saturating]")
{
    SECTION("signed short type")
    {
        using type = strong::type<short, short, strong::saturating_arithmetic>;

        auto max = std::numeric_limits<short>::max();
        auto min = std::numeric_limits<short>::min();

        REQUIRE(value_of(type{max + 1L}) == max);
        REQUIRE(value_of(type{min - 1L}) == min);
        REQUIRE(value_of(type{max - 1L}) == max - 1L);
        REQUIRE(value_of(type{min + 1L}) == min + 1L);
    }
    SECTION("unsigned short type")
    {
        using type = strong::type<unsigned short, unsigned short, strong::saturating_arithmetic>;

        auto max = std::numeric_limits<unsigned short>::max();
        auto min = std::numeric_limits<unsigned short>::min();

        REQUIRE(value_of(type{max + 1L}) == max);
        REQUIRE(value_of(type{min - 1L}) == min);
        REQUIRE(value_of(type{max - 1L}) == max - 1L);
        REQUIRE(value_of(type{min + 1L}) == min + 1L);
    }
}

TEST_CASE("saturating_cast", "[saturating]")
{
    using ltype = strong::type<int32_t, int32_t, strong::saturating_arithmetic>;
    using utype = strong::type<uint16_t, uint16_t, strong::saturating_arithmetic>;
    using stype = strong::type<int16_t, int16_t, strong::saturating_arithmetic>;

    auto smax = std::numeric_limits<int16_t>::max();

    SECTION("from large to small")
    {
        auto smin = std::numeric_limits<int16_t>::min();

        REQUIRE(value_of(strong::saturating_cast<stype>(ltype(smax + 1))) == smax);
        REQUIRE(value_of(strong::saturating_cast<stype>(ltype(smin - 1))) == smin);
    }
    SECTION("from signed to unsigned")
    {
        REQUIRE(value_of(strong::saturating_cast<utype>(stype(smax))) == smax);
        REQUIRE(value_of(strong::saturating_cast<utype>(stype(-1))) == 0);
    }
    SECTION("from unsigned to signed")
    {
        REQUIRE(value_of(strong::saturating_cast<stype>(utype(smax + 1))) == smax);
    }
    SECTION("from fundamental type")
    {
        REQUIRE(value_of(strong::saturating_cast<utype>(-1)) == 0);
    }
}
#endif