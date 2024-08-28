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

#include <strong_type/iostreamable.hpp>

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <sstream>
#endif

TEST_CASE("an iostreamable type can be both read and written using streams")
{
    strong::type<int, struct i_, strong::iostreamable> i{0};
    std::istringstream is{"3"};
    is >> i;
    std::ostringstream os;
    os << i;
    REQUIRE(os.str() == "3");
}
