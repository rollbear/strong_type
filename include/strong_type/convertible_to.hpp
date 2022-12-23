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

#ifndef STRONG_TYPE_CONVERTIBLE_TO_HPP
#define STRONG_TYPE_CONVERTIBLE_TO_HPP

#include "type.hpp"

namespace strong
{
namespace impl
{
template<typename T, typename D>
struct converter
{
    STRONG_CONSTEXPR explicit operator D() const
    noexcept(noexcept(static_cast<D>(std::declval<const underlying_type_t<T>&>())))
    {
        auto& self = static_cast<const T&>(*this);
        return static_cast<D>(value_of(self));
    }
};

}

template <typename ... Ts>
struct convertible_to
{
    template <typename T>
    struct modifier : impl::converter<T, Ts>...
    {
    };
};

}
#endif //STRONG_TYPE_CONVERTIBLE_TO_HPP
