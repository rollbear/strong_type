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

#include <strong_type/hashable.hpp>
#include <strong_type/regular.hpp>

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <unordered_set>
#endif

TEST_CASE("strong::hashable can be used in unordered_set")
{
    using hs = strong::type<std::string, struct hs_, strong::hashable, strong::regular>;

    using sset = std::unordered_set<hs>;

    sset strings{hs{"foo"}, hs{"bar"}};

    REQUIRE(strings.find(hs{"foo"}) != strings.end());
    REQUIRE(strings.find(hs{"bar"}) != strings.end());
    REQUIRE(strings.find(hs{"baz"}) == strings.end());
}
