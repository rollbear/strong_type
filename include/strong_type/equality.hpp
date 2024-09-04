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

#ifndef STRONG_TYPE_EQUALITY_HPP
#define STRONG_TYPE_EQUALITY_HPP

#include "type.hpp"

namespace strong
{
STRONG_TYPE_MODULE_EXPORT struct equality
{
    template <typename T, typename = void>
    class modifier {
        static_assert(impl::always_false<T>,
                      "Underlying type must be equality comparable");
    };
};


STRONG_TYPE_MODULE_EXPORT template <typename T, typename Tag, typename ... M>
class equality::modifier<
    ::strong::type<T, Tag, M...>,
    impl::void_t<decltype(std::declval<const T&>() == std::declval<const T&>())>
>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator==(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()))
    -> decltype(std::declval<const T&>() == std::declval<const T&>())
    {
        return value_of(lh) == value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator!=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() != std::declval<const T&>()))
    -> decltype(std::declval<const T&>() != std::declval<const T&>())
    {
        return value_of(lh) != value_of(rh);
    }
};

}
#endif //STRONG_TYPE_EQUALITY_HPP
