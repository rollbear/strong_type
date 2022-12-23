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

#include <strong_type/unique.hpp>

#include "catch2.hpp"

#include <memory>

TEST_CASE("strong::unique is movable")
{
    using t = strong::type<std::unique_ptr<int>, struct t_, strong::unique>;

    GIVEN("an initialized source")
    {
        t source{std::make_unique<int>(3)};
        auto* addr = value_of(source).get();

        WHEN("move constructing a dest")
        {
            auto dest = std::move(source);
            THEN("the source is moved from")
            {
                REQUIRE(value_of(source).get() == nullptr);
            }
            AND_THEN("the dest has the value that source had")
            {
                REQUIRE(*value_of(dest) == 3);
                REQUIRE(value_of(dest).get() == addr);
            }
        }
        AND_WHEN("move assigning to another instance")
        {
            t dest{std::make_unique<int>(4)};
            dest = std::move(source);
            THEN("the source is moved from")
            {
                REQUIRE(value_of(source).get() == nullptr);
            }
            AND_THEN("the dest has the value that source had")
            {
                REQUIRE(*value_of(dest) == 3);
                REQUIRE(value_of(dest).get() == addr);
            }
        }
    }
}
