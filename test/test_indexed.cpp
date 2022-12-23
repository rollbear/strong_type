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

#include <strong_type/indexed.hpp>

#include "catch2.hpp"

#include "test_utils.hpp"

TEST_CASE("indexed can be accessed using operator [] and .at()")
{
    using I = strong::type<unsigned, struct I_>;
    using T = strong::type<std::string, struct s_, strong::indexed<>>;
    using TI = strong::type<std::string, struct s_, strong::indexed<I>>;

    T s("foo");
    const T c("bar");
    TI si("foo");
    const TI ci("bar");

    auto& r = s[I{0U}];
    static_assert(!std::is_const<std::remove_reference_t<decltype(r)>>{}, "");
    REQUIRE(r == 'f');
    auto& ri = si[I{0U}];
    static_assert(!std::is_const<std::remove_reference_t<decltype(ri)>>{}, "");
    REQUIRE(ri == 'f');
    auto& cr = c[I{0U}];
    static_assert(std::is_const<std::remove_reference_t<decltype(cr)>>{}, "");
    REQUIRE(cr == 'b');
    auto& cri = ci[I{0U}];
    static_assert(std::is_const<std::remove_reference_t<decltype(cri)>>{}, "");
    REQUIRE(cri == 'b');

    auto& ar = s.at(I{0U});
    static_assert(!std::is_const<std::remove_reference_t<decltype(ar)>>{}, "");
    REQUIRE(ar == 'f');
    auto& ari = si.at(I{0U});
    static_assert(!std::is_const<std::remove_reference_t<decltype(ari)>>{}, "");
    REQUIRE(ari == 'f');
    auto& acr = c.at(I{0U});
    static_assert(std::is_const<std::remove_reference_t<decltype(acr)>>{}, "");
    REQUIRE(acr == 'b');
    auto& acri = ci.at(I{0U});
    static_assert(std::is_const<std::remove_reference_t<decltype(acri)>>{}, "");
    REQUIRE(acri == 'b');

    // operator[] on std::string returns lvalue reference
    static_assert(std::is_same<decltype(std::declval<std::string&&>()[1]), char&>{},"");
    // hence ...

    auto&& tmp = std::move(s)[1U];
    STATIC_REQUIRE(std::is_same<decltype(tmp), char&>{});
    REQUIRE(tmp == 'o');

    auto&& tmpat = std::move(s).at(1U);
    STATIC_REQUIRE(std::is_same<decltype(tmpat), char&>{});
    REQUIRE(tmpat == 'o');


    auto&& tmpi = std::move(si)[I{1U}];
    STATIC_REQUIRE(std::is_same<decltype(tmpi), char&>{});
    REQUIRE(tmpi == 'o');

    auto&& tmpiat = std::move(si).at(I{1U});
    STATIC_REQUIRE(std::is_same<decltype(tmpiat), char&>{});
    REQUIRE(tmpiat == 'o');
}

template <typename T, size_t N>
class simple_array
{
public:
    T& operator[](int x) & { return data_[x];}
    T&& operator[](int x) && { return std::move(data_[x]);}
    const T& operator[](int x) const & { return data_[x];}
    const T& operator[](int x) const && { return data_[x];}
private:
    T data_[N] = {};
};

template <typename T, size_t N>
using array = strong::type<simple_array<T, N>, struct array_,
    strong::default_constructible,
    strong::indexed<int>>;
template <typename ...> struct S;
TEST_CASE("indexed can be accessed with operator[] without at()")
{
    using A = array<int, 3>;

    A a{};
    const A ca{};

    static_assert(std::is_same<decltype(a[1]), int&>{}, "");
    static_assert(std::is_same<decltype(ca[1]), const int&>{}, "");
    static_assert(std::is_same<decltype(std::move(a)[1]), int&&>{}, "");
    static_assert(std::is_same<decltype(std::move(ca)[1]), const int&>{}, "");

    a[0] = 3;
    a[1] = 4;
    a[2] = 5;

    REQUIRE(as_const(a)[0] == 3);
    REQUIRE(a[1] == 4);
    REQUIRE(std::move(as_const(a))[2] == 5);
}
