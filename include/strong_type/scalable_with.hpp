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
        template <
            typename TT = T,
            typename UT = underlying_type_t<TT>,
            typename R = decltype(std::declval<const UT&>() / std::declval<const UT&>())
        >
        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        auto operator/(const T& lh, const T& rh)
        noexcept(noexcept(std::declval<const UT&>() / std::declval<const UT&>()))
        -> R
        {
            return value_of(lh) / value_of(rh);
        }
    };
};

}
#endif //STRONG_TYPE_SCALABLE_HPP
