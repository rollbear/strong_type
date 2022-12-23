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

#ifndef STRONG_TYPE_SEMIREGULAR_HPP
#define STRONG_TYPE_SEMIREGULAR_HPP

#include "type.hpp"

namespace strong
{
namespace impl
{

template <typename T>
struct require_semiregular
    : valid_type<require_copy_constructible<T>::value &&
                 require_move_constructible<T>::value &&
                 require_copy_assignable<T>::value &&
                 require_move_assignable<T>::value>
{
};

}
struct semiregular
{
    template <typename>
    class modifier;
};

template <typename T, typename Tag, typename ... M>
class semiregular::modifier<::strong::type<T, Tag, M...>>
    : public default_constructible::modifier<T>
        , private impl::require_semiregular<T>
{
};

}
#endif //STRONG_TYPE_SEMIREGULAR_HPP
