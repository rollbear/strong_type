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

#include <strong_type/scalable_with.hpp>

namespace {
struct line {
    line(int x_) : x(x_)
    {}

    line &operator/=(int y)
    {
        x /= y;
        return *this;
    }
    friend line operator/(line lh, int rh)
    {
        return lh /= rh;
    }
    line &operator*=(int y)
    {
        x *= y;
        return *this;
    }
    friend line operator*(line lh, int rh)
    {
        return lh *= rh;
    }
    int operator/(line rh) const
    {
        return x / rh.x;
    }

    int x;
};

struct plane {
    plane(int x_, int y_) : x(x_), y(y_)
    {}

    plane &operator/=(int f)
    {
        x /= f;
        y /= f;
        return *this;
    }
    friend plane operator/(plane lh, int rh)
    {
        return lh /= rh;
    }
    plane &operator*=(int f)
    {
        x *= f;
        y *= f;
        return *this;
    }
    friend plane operator*(plane lh, int rh)
    {
        return lh *= rh;
    }
    plane &operator/=(double d)
    {
        x = static_cast<int>(x / d);
        y = static_cast<int>(y / d);
        return *this;
    }
    friend plane operator/(plane lh, double rh)
    {
        return lh /= rh;
    }
    int x;
    int y;
};

}

TEST_CASE("one dimensional")
{
    GIVEN("a scalable int, scalable with int")
    {
        using lt = strong::type<line, struct lt_, strong::scalable_with<int>>;
        lt var(30);
        WHEN("mulltiplying by an int")
        {
            auto v1 = var * 2;
            auto v2 = 2 * var;
            THEN("dhe dimension is multiplied")
            {
                REQUIRE(value_of(v1).x == 60);
                REQUIRE(value_of(v2).x == 60);
            }
        }
        AND_WHEN("dividing by an int")
        {
            auto w = var / 2;
            THEN("the dimension is divided")
            {
                REQUIRE(value_of(w).x == 15);
            }
        }
        AND_WHEN("dividing by another scalable of same type")
        {
            auto x = var / lt(5);
            THEN("the result is an int")
            {
                STATIC_REQUIRE(std::is_same<int, decltype(x)>::value);
            }
            AND_THEN("the value is the result of dividing the dimensins")
            {
                REQUIRE(x == 6);
            }
        }
    }
}

TEST_CASE("two dimensional")
{
    GIVEN("a dwo dimensional scalable, scalable with int, double and other scalable type")
    {
        using si = strong::type<int, struct si_>;
        using pt = strong::type<plane, struct pt_, strong::scalable_with<int, double, si>>;

        pt p{30, 40};
        WHEN("multiplied by an int")
        {
            auto v1 = p * 2;
            auto v2 = 2 * p;
            THEN("the value of each dimension is multiplied")
            {
                REQUIRE(value_of(v1).x == 60);
                REQUIRE(value_of(v1).y == 80);
                REQUIRE(value_of(v2).x == 60);
                REQUIRE(value_of(v2).y == 80);
            }
        }
        AND_WHEN("divided by an int")
        {
            auto w = p / 2;
            THEN("the value of each dimension is divided")
            {
                REQUIRE(value_of(w).x == 15);
                REQUIRE(value_of(w).y == 20);
            }
        }
        AND_WHEN("divided by a floating point number")
        {
            auto y = p / 0.5;
            THEN("the value of each dimension is divided")
            {
                REQUIRE(value_of(y).x == 60);
                REQUIRE(value_of(y).y == 80);
            }
        }
        AND_WHEN("divided by another scalable")
        {
            auto z = p / si{2};
            THEN("the value of each dimension is divided")
            {
                REQUIRE(value_of(z).x == 15);
                REQUIRE(value_of(z).y == 20);
            }
        }
    }
}

TEST_CASE("integral promotion")
{
    /*
     * The primary purpose of this test is to ensure that it
     *  builds without warnings when compiled with -Wconversion
     *  and without complaint from -fsanitize=undefined
     */
    GIVEN("a scalable short, scalable by short")
    {
        using ss = strong::type<short, struct ss_, strong::scalable_with<short>>;
        ss val{short(30)};
        WHEN("divided by a short")
        {
            val /= short(2);
            THEN("the value is divided")
            {
                REQUIRE(value_of(val) == 15);
            }
        }
        AND_WHEN("multiplied by a short")
        {
            val *= short(3);
            THEN("the value is multiplied")
            {
                REQUIRE(value_of(val) == 90);
            }
        }
    }
}