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
#include <sstream>

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

#if defined(STRONG_TYPE_HAS_RANGES)
#if (!defined(_LIBCPP_VERSION)) || (_LIBCPP_VERSION >= 16000)
TEST_CASE("a range with a sentinel can be iterated")
{
    constexpr auto iteration = [](auto&& r) {
        int i = 1;
        for (auto x : r) {
            REQUIRE(x == i);
            ++i;
        }
        REQUIRE(i == 4);
    };
    GIVEN("a non-const strong vector") {
        using va = strong::type<std::vector<int>, struct va_, strong::range>;
        va numbers{1, 2, 3, 4, 5};
        THEN("a ranges view can be used to iterate over it") {
            iteration(numbers | std::ranges::views::take(3));
        }
        AND_THEN("a filter view can iterate over it") {
            iteration(numbers | std::ranges::views::filter([](auto x) { return x < 4;}));
        }
    }
    AND_GIVEN("a const strong vector") {
        using va = strong::type<std::vector<int>, struct va_, strong::range>;
        const va numbers{1, 2, 3, 4, 5};
        THEN("a ranges view can be used to iterate over it") {
            iteration(numbers | std::ranges::views::take(3));
        }
        AND_THEN("a filter view can iterate over it") {
            iteration(numbers | std::ranges::views::filter([](auto x) { return x < 4;}));
        }
    }
    AND_GIVEN("a filter view over a const vector") {
        const std::vector input{1,2,3,4,5};
        auto filter = input | std::ranges::views::filter([](auto x){return x < 4;});
        auto sf = strong::type<decltype(filter), struct f_, strong::range>(filter);
        THEN("the filter view can iterate over it") {
            iteration(sf);
        }
    }
    AND_GIVEN("a filter view over a non-const vector") {
        std::vector input{1,2,3,4,5};
        auto filter = input | std::ranges::views::filter([](auto x){return x < 4;});
        auto sf = strong::type<decltype(filter), struct f_, strong::range>(filter);
        THEN("the filter view can iterate over it") {
            iteration(sf);
        }
    }
    AND_GIVEN("a non-const forward_list") {
        std::forward_list input{1,2,3,4,5};
        WHEN("a filter view is created from it") {
            auto filter = input | std::ranges::views::filter([](auto x) { return x < 4; });
            auto sf = strong::type<decltype(filter), struct f_, strong::range>(filter);
            THEN("the filter view can iterate over it")
            {
                iteration(sf);
            }
        }
        AND_WHEN("a take view is created from it") {
            auto first3 = input | std::ranges::views::take(3);
            auto sf = strong::type<decltype(first3), struct f_, strong::range>(first3);
            THEN("the first3 view can iterate over it")
            {
                iteration(sf);
            }
        }
    }
    AND_GIVEN("a non-const forward_list") {
        std::forward_list input{1,2,3,4,5};
        AND_WHEN("a const take view is created from it") {
            auto first3 = input | std::ranges::views::take(3);
            auto const sf = strong::type<decltype(first3), struct f_, strong::range>(first3);
            THEN("the first3 view can iterate over it")
            {
                iteration(sf);
            }
        }
    }
    AND_GIVEN("a const forward_list") {
        const std::forward_list input{1,2,3,4,5};
        WHEN("a filter view is created from it") {
            auto filter = input | std::ranges::views::filter([](auto x) { return x < 4; });
            auto sf = strong::type<decltype(filter), struct f_, strong::range>(filter);
            THEN("the filter view can iterate over it")
            {
                iteration(sf);
            }
        }
        AND_WHEN("a take view is created from it") {
            auto first3 = input | std::ranges::views::take(3);
            auto sf = strong::type<decltype(first3), struct f_, strong::range>(first3);
            THEN("the first3 view can iterate over it")
            {
                iteration(sf);
            }
        }
        AND_WHEN("a const take view is created from it") {
            auto first3 = input | std::ranges::views::take(3);
            const auto sf = strong::type<decltype(first3), struct f_, strong::range>(first3);
            THEN("the first3 view can iterate over it")
            {
                iteration(sf);
            }
        }
    }

#if __cpp_lib_ranges >= 202110L
    AND_GIVEN("a string with numbers")
    {
        std::istringstream is("1 2 3");
        WHEN("a stream view is created from it") {
            auto stream = std::views::istream<int>(is);
            auto sf = strong::type<decltype(stream), struct f_, strong::range>(stream);
            THEN("the stream can be iterated over") {
                iteration(sf);
            }
        }
    }
#endif
}
#endif

#endif
