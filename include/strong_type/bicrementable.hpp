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

#ifndef STRONG_TYPE_BICREMENTABLE_HPP
#define STRONG_TYPE_BICREMENTABLE_HPP

#include "incrementable.hpp"
#include "decrementable.hpp"

namespace strong
{
struct bicrementable
{
    template <typename T>
    class modifier
        : public incrementable::modifier<T>
        , public decrementable::modifier<T>
    {
    };
};

}
#endif //STRONG_TYPE_BICREMENTABLE_HPP
