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

#include <strong_type/iterator.hpp>

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <unordered_set>
#include <vector>
#include <algorithm>
#endif

TEST_CASE("iterators work with algorithms")
{
    std::unordered_set<int> is{3,2,8,4,11,9,22,23};
    using si = strong::type<std::unordered_set<int>::iterator, struct si_, strong::iterator>;
    si sb{is.begin()};
    si se{is.end()};

    std::vector<int> v;
    std::copy(sb, se, std::back_inserter(v));

    using vi = strong::type<std::vector<int>::iterator, struct vi_, strong::iterator>;

    vi vb{v.begin()};
    vi ve{v.end()};
    std::sort(vb, ve);
    REQUIRE(vb[0] == 2);
    REQUIRE(vb[7] == 23);
}
