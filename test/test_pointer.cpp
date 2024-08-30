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

#include <strong_type/pointer.hpp>

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <memory>
#endif

TEST_CASE("pointer types can be compared with nullptr")
{
    using P = strong::type<std::unique_ptr<int>, struct p_, strong::pointer>;

    P p{nullptr};
    REQUIRE(p == nullptr);
    REQUIRE(nullptr == p);
    REQUIRE_FALSE(p != nullptr);
    REQUIRE_FALSE(nullptr != p);

    p = P{std::make_unique<int>(3)};

    REQUIRE_FALSE(p == nullptr);
    REQUIRE_FALSE(nullptr == p);
    REQUIRE(p != nullptr);
    REQUIRE(nullptr != p);
}

TEST_CASE("pointer types can be dereferenced using operator ->")
{
    struct S { int i;};
    S s {3};
    strong::type<S*, struct s_, strong::pointer> p{&s};

    REQUIRE(p->i == 3);
}

TEST_CASE("pointer types can be dereferenced using operator *")
{
    int i = 0;
    strong::type<int*, struct s_, strong::pointer> p{&i};
    *p = 3;
    REQUIRE(i == 3);
}
