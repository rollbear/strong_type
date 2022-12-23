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

#ifndef STRONG_TYPE_ORDERED_HPP
#define STRONG_TYPE_ORDERED_HPP

#include "type.hpp"

namespace strong
{
struct ordered
{
    template <typename T>
    class modifier;
};


template <typename T, typename Tag, typename ... M>
class ordered::modifier<::strong::type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() < std::declval<const T&>()))
    -> decltype(std::declval<const T&>() < std::declval<const T&>())
    {
        return value_of(lh) < value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() <= std::declval<const T&>()))
    -> decltype(std::declval<const T&>() <= std::declval<const T&>())
    {
        return value_of(lh) <= value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator>(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() > std::declval<const T&>()))
    -> decltype(std::declval<const T&>() > std::declval<const T&>())
    {
        return value_of(lh) > value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR

    auto
    operator>=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() >= std::declval<const T&>()))
    -> decltype(std::declval<const T&>() >= std::declval<const T&>())
    {
        return value_of(lh) >= value_of(rh);
    }
};

}

#endif //STRONG_TYPE_ORDERED_HPP
