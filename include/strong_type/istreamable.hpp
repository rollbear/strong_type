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

#ifndef STRONG_TYPE_ISTREAMABLE_HPP
#define STRONG_TYPE_ISTREAMABLE_HPP

#include "type.hpp"

#include <istream>

namespace strong
{
struct istreamable
{
    template <typename T>
    class modifier
    {
    public:
        friend
        std::istream&
        operator>>(
            std::istream &is,
            T &t)
        {
            return is >> value_of(t);
        }
    };
};

}
#endif //STRONG_TYPE_ISTREAMABLE_HPP
