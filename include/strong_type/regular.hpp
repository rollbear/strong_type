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

#ifndef STRONG_TYPE_REGULAR_HPP
#define STRONG_TYPE_REGULAR_HPP

#include "equality.hpp"
#include "semiregular.hpp"

namespace strong
{
STRONG_TYPE_MODULE_EXPORT struct regular
{
    template <typename T>
    class modifier
        : public semiregular::modifier<T>
        , public equality::modifier<T>
    {
    };
};

}
#endif //STRONG_TYPE_REGULAR_HPP
