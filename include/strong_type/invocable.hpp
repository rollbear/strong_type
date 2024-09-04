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


#ifndef STRONG_TYPE_INVOCABLE_HPP
#define STRONG_TYPE_INVOCABLE_HPP

#include "type.hpp"

namespace strong
{

STRONG_TYPE_MODULE_EXPORT struct invocable
{
    template <typename>
    class modifier;
};

STRONG_TYPE_MODULE_EXPORT template <typename T, typename Tag, typename ... Ms>
class invocable::modifier<type<T, Tag, Ms...>>
{
    using type = strong::type<T, Tag, Ms...>;
public:
    template <typename ... Args, typename R = decltype(std::declval<T&>()(std::declval<Args>()...))>
    STRONG_CONSTEXPR
    R operator()(Args&& ... args) &
    noexcept (noexcept(std::declval<T&>()(std::declval<Args>()...)))
    {
        return call(static_cast<type&>(*this), std::forward<Args>(args)...);
    }
    template <typename ... Args, typename R = decltype(std::declval<T&&>()(std::declval<Args>()...))>
    STRONG_CONSTEXPR
    R operator()(Args&& ... args) &&
    noexcept (noexcept(std::declval<T&&>()(std::declval<Args>()...)))
    {
        return call(static_cast<type&&>(std::move(*this)), std::forward<Args>(args)...);
    }
    template <typename ... Args, typename R = decltype(std::declval<const T&>()(std::declval<Args>()...))>
    STRONG_CONSTEXPR
    R operator()(Args&& ... args) const &
    noexcept (noexcept(std::declval<const T&>()(std::declval<Args>()...)))
    {
        return call(static_cast<const type&>(*this), std::forward<Args>(args)...);
    }
    template <typename ... Args, typename R = decltype(std::declval<const T&&>()(std::declval<Args>()...))>
    STRONG_CONSTEXPR
    R operator()(Args&& ... args) const &&
    noexcept (noexcept(std::declval<const T&&>()(std::declval<Args>()...)))
    {
        return call(static_cast<const type&&>(std::move(*this)), std::forward<Args>(args)...);
    }
private:
    template <typename Self, typename ... Ts>
    static STRONG_CONSTEXPR decltype(auto) call(Self&& self, Ts&& ... ts)
    {
        return value_of(std::forward<Self>(self))(std::forward<Ts>(ts)...);
    }
};

}
#endif //STRONG_TYPE_INVOCABLE_HPP
