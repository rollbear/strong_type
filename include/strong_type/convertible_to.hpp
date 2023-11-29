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
template<typename T, typename D, typename = void>
struct converter
{
    static_assert(always_false<T, D>,
                  "Underlying type must be convertible to target type");
};
template <typename T, typename Tag, typename ... Ms, typename D>
struct converter<
    strong::type<T, Tag, Ms...>,
    D,
    impl::void_t<decltype(static_cast<D>(std::declval<const T&>()))>
>
{
    STRONG_CONSTEXPR explicit operator D() const
    noexcept(noexcept(static_cast<D>(std::declval<const T&>())))
    {
        auto& self = static_cast<const strong::type<T, Tag, Ms...>&>(*this);
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
