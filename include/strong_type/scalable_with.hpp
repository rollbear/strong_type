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

#ifndef STRONG_TYPE_SCALABLE_HPP
#define STRONG_TYPE_SCALABLE_HPP

#include "type.hpp"

namespace strong
{
namespace impl
{
template <typename ...>
struct first_type;
template <typename T>
struct first_type<T>
{
    using type = T;
};
template <typename T, typename ...Ts>
struct first_type<T, Ts...> {
    using type = T;
};
template <typename ... Ts>
using first_type_t = typename first_type<Ts...>::type;

template <typename T, typename Other>
class typed_scalable
{
    using TT = underlying_type_t<T>;
    using OT = underlying_type_t<Other>;
public:
    friend
    STRONG_CONSTEXPR
    auto operator/=(T& lh, const Other& rh)
    noexcept(noexcept(std::declval<TT&>() / std::declval<const OT&>())
             && std::is_nothrow_move_assignable<TT>::value)
    -> T&
    {
        value_of(lh) = static_cast<TT>(value_of(lh) / impl::access(rh));
        return lh;
    }
    friend
    STRONG_CONSTEXPR
    auto operator*=(T& lh, const Other& rh)
    noexcept(noexcept(std::declval<TT&>() * std::declval<const OT&>())
             && std::is_nothrow_move_assignable<TT>::value)
    -> T&
    {
        value_of(lh) = static_cast<TT>(value_of(lh) * impl::access(rh));
        return lh;
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator/(T lh, const Other& rh)
    noexcept(noexcept(lh /= rh))
    -> T
    {
        lh /= rh;
        return lh;
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator*(T lh, const Other& rh)
    noexcept(noexcept(lh *= rh))
    -> T
    {
        lh *= rh;
        return lh;
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator*(const Other& lh, T rh)
    noexcept(noexcept(rh *= lh))
    -> T
    {
        rh *= lh;
        return rh;
    }
};
}

template <typename ... Ts>
struct scalable_with
{
    template <typename T>
    class modifier : public impl::typed_scalable<T, Ts>...
    {
    public:
        using scalable_modifier_result_type = impl::first_type_t<Ts...>;
    };
};
}

template <
    typename TT,
    typename UT = strong::underlying_type_t<TT>,
    typename R = typename TT::scalable_modifier_result_type,
    typename = std::enable_if_t<std::is_constructible<R, decltype(std::declval<const UT&>() / std::declval<const UT&>())>::value>
>
STRONG_NODISCARD
inline
STRONG_CONSTEXPR
auto operator/(const TT& lh, const TT& rh)
noexcept(noexcept(std::declval<const UT&>() / std::declval<const UT&>()))
-> R
{
    return R{value_of(lh) / value_of(rh)};
}

#endif //STRONG_TYPE_SCALABLE_HPP
