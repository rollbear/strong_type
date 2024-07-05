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

#include "strong_type/formattable.hpp"

#include "catch2.hpp"

struct so {
    so(int v) : i(v) {}
    int i;
    friend std::ostream& operator<<(std::ostream& os, const so& obj)
    {
        return os << "os << " << obj.i;
    }
};

template <typename Char>
struct fmt::formatter<so, Char> : fmt::formatter<int, Char>
{
    template <typename FormatContext>
    decltype(auto)
    format(const so& t, FormatContext& fc) const
    {
        return formatter<int, Char>::format(t.i, fc);
    }
};

using osso = strong::type<so, struct osso_, strong::ostreamable>;
using fmtso = strong::type<so, struct fmtso_, strong::formattable>;
using fmtosso = strong::type<so, struct fmtf_, strong::formattable, strong::ostreamable>;

TEST_CASE("ostreamable is formattable")
{
    osso val{3};
    auto s = fmt::format("{}", val);
    REQUIRE(s == "os << 3");
}
TEST_CASE("formattable is formattable")
{
    fmtso val{3};
    auto s = fmt::format("{}", val);
    REQUIRE(s == "3");
}

TEST_CASE("formattable and ostreamable is formattable as formattable")
{
    fmtosso val{3};
    auto s = fmt::format("{}", val);
    REQUIRE(s == "3");
}
