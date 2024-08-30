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

#include <strong_type/difference.hpp>

TEST_CASE("adding difference types yields a difference type")
{
    using U = strong::type<int, struct u_, strong::difference>;

    U u1{3};
    U u2{4};

    auto r = u1 + u2;
    static_assert(std::is_same<decltype(r), U>{},"");
    REQUIRE(value_of(r) == 7);
}

TEST_CASE("subtracting difference types yields a difference type")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u1{8};
    U u2{3};

    auto r = u1 - u2;

    static_assert(std::is_same<decltype(r), U>{}, "");
    REQUIRE(value_of(r) == 5);
}

TEST_CASE("dividing difference types yields a base type")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u1{8};
    U u2{2};

    auto r = u1/u2;
    static_assert(std::is_same<decltype(r), int>{}, "");
    REQUIRE(r == 4);
}

TEST_CASE("remainder after division between difference types yields a base type")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u1{15};
    U u2{6};

    auto r = u1 % u2;

    static_assert(std::is_same<decltype(r), int>{},"");
    REQUIRE(r == 3);
}

TEST_CASE("remainder after division between difference type and base type yields a difference type")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u{15};

    auto r = u % 6;

    static_assert(std::is_same<decltype(r), U>{}, "");
    REQUIRE(r == U{3});
}

TEST_CASE("dividing a difference type with its base type yields a difference")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u{8};

    auto r = u/2;
    static_assert(std::is_same<decltype(r), U>{}, "");
    REQUIRE(value_of(r) == 4);
}

TEST_CASE("difference types are ordered and equality comparable")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u1{1};
    U u2{2};
    U u3{1};

    REQUIRE(u1 == u3);
    REQUIRE(!(u1 == u2));
    REQUIRE(u1 != u2);
    REQUIRE(!(u1 != u3));
    REQUIRE(u1 < u2);
    REQUIRE(!(u1 < u3));
    REQUIRE(u1 <= u2);
    REQUIRE(u1 <= u3);
    REQUIRE(!(u2 <= u1));
    REQUIRE(u2 > u1);
    REQUIRE(!(u3 > u1));
    REQUIRE(!(u3 > u2));
    REQUIRE(u3 >= u1);
    REQUIRE(u3 >= u1);
    REQUIRE(!(u3 >= u2));
}

TEST_CASE("multiplying a difference with its base type yields a difference")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u{3};

    auto r1 = u * 2;
    static_assert(std::is_same<decltype(r1), U>{}, "");
    REQUIRE(value_of(r1) == 6);

    auto r2 = 3 * u;
    static_assert(std::is_same<decltype(r2), U>{}, "");
    REQUIRE(value_of(r2) == 9);
}

namespace {
struct S {
    int v;

    S(int v_) : v(v_) {}
    template <typename ...>
    S &operator+=(const S &rh) { v += rh.v; return *this; }
    template <typename...>
    friend S operator+(S lh, const S &rh) { return lh += rh; }
    template <typename...>
    S &operator-=(const S &rh) { v -= rh.v; return *this; }
    template <typename...>
    friend S operator-(S lh, const S &rh) { return lh -= rh; }
    template <typename...>
    friend bool operator==(const S &lh, const S &rh) { return lh.v == rh.v; }
    template <typename...>
    friend bool operator!=(const S &lh, const S &rh) { return lh.v != rh.v; }
};
}

TEST_CASE("an equality comparable arithmetic type without ordering can be a difference")
{
    using type = strong::type<S, struct S_, strong::difference>;
    REQUIRE(type{3} - type{2} == type{1});
}