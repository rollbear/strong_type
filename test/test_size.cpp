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
#include <vector>

#include <strong_type/regular.hpp>
#include <strong_type/range.hpp>
#include <strong_type/equality.hpp>
#include <strong_type/ordered.hpp>
#include <strong_type/arithmetic.hpp>
#include <strong_type/bicrementable.hpp>
#include <strong_type/iostreamable.hpp>
#include <strong_type/difference.hpp>
#include <strong_type/equality_with.hpp>
#include <strong_type/ordered_with.hpp>
#include <strong_type/convertible_to.hpp>
#include <strong_type/scalable_with.hpp>

namespace {
// a single modifier: one empty base, folded by every ABI
using one_leaf = strong::type<int, struct one_leaf_, strong::equality>;

// several empty modifier bases: the case MSVC does not fold without help
using two_leaves = strong::type<int, struct two_leaves_, strong::equality, strong::ordered>;
using many_leaves = strong::type<double, struct many_leaves_, strong::arithmetic, strong::ordered, strong::equality>;

// composite modifiers, whose own modifier<T> has multiple empty bases
using is_regular = strong::type<int, struct is_regular_, strong::regular>;
using streamable = strong::type<int, struct streamable_, strong::iostreamable>;
using bicrement = strong::type<int, struct bicrement_, strong::bicrementable>;
using diff = strong::type<int, struct diff_, strong::difference>;

// variadic composite modifiers with more than one target type
using eq_with = strong::type<int, struct eq_with_, strong::equality_with<int, long>>;
using ord_with = strong::type<int, struct ord_with_, strong::ordered_with<int, long>>;
using conv_to = strong::type<int, struct conv_to_, strong::convertible_to<long, long long>>;
using scale_with = strong::type<int, struct scale_with_, strong::scalable_with<int, long>>;

using range = strong::type<std::vector<int>, struct range_, strong::range, strong::equality>;
}

TEST_CASE("a strong::type is the same size as its underlying type")
{
    STATIC_REQUIRE(sizeof(one_leaf) == sizeof(int));
    STATIC_REQUIRE(sizeof(two_leaves) == sizeof(int));
    STATIC_REQUIRE(sizeof(many_leaves) == sizeof(double));
    STATIC_REQUIRE(sizeof(is_regular) == sizeof(int));
    STATIC_REQUIRE(sizeof(streamable) == sizeof(int));
    STATIC_REQUIRE(sizeof(bicrement) == sizeof(int));
    STATIC_REQUIRE(sizeof(diff) == sizeof(int));
    STATIC_REQUIRE(sizeof(eq_with) == sizeof(int));
    STATIC_REQUIRE(sizeof(ord_with) == sizeof(int));
    STATIC_REQUIRE(sizeof(conv_to) == sizeof(int));
    STATIC_REQUIRE(sizeof(scale_with) == sizeof(int));
    STATIC_REQUIRE(sizeof(range) == sizeof(std::vector<int>));
}
