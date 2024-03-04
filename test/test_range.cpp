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

#include <forward_list>
#include <array>

namespace {
    template <typename T>
    constexpr bool ignore(const T&) { return true;}
    template<typename R, typename = bool>
    struct has_size : std::false_type {
    };

    template<typename R>
    struct has_size<R, decltype(ignore(std::declval<const R&>().size()))>
    : std::true_type
    {
    };
    using v = strong::type<std::vector<int>, struct v_, strong::range>;
    static_assert(has_size<v>{}, "");
    using f = strong::type<std::forward_list<int>, struct f_, strong::range>;
    static_assert(!has_size<f>{}, "");
}

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

#if __cplusplus >= 201703L
TEST_CASE("begin() and end() are constexpr")
{
    using ia = strong::type
            <
                    std::array<int, 4>,
                    struct ia_,
                    strong::range,
                    strong::default_constructible
            >;
    static constexpr ia a{};
    STATIC_REQUIRE(a.end() == a.begin() + 4);
    STATIC_REQUIRE(a.cend() == a.cbegin() + 4);
    STATIC_REQUIRE(*a.begin() == 0);
}
#endif

TEST_CASE("range of sized type has size")
{
    using iv = strong::type<std::vector<int>, struct vi_, strong::range>;

    iv v{3,2,1, 0};
    REQUIRE(v.size() == 4);
}

TEST_CASE("constexpr size")
{
    using ia = strong::type
            <
                    std::array<int, 4>,
                    struct ia_,
                    strong::range,
                    strong::default_constructible
            >;

    static constexpr ia a{};
    REQUIRE(a.size() == 4);
    STATIC_REQUIRE(a.size() == 4);
}