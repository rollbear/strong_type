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

#include <strong_type/ostreamable.hpp>

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <sstream>
#include <iomanip>
#endif

TEST_CASE("an ostreamable type can be streamed using stream flags")
{
    strong::type<int, struct i_, strong::ostreamable> i{3};

    std::ostringstream oss;
    oss << std::setw(4) << i;
    REQUIRE(oss.str() == "   3");
}
