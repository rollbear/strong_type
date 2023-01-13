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

#ifndef STRONG_TYPE_ARITHMETIC_HPP
#define STRONG_TYPE_ARITHMETIC_HPP

#include "type.hpp"

#include <limits>

namespace strong
{
struct arithmetic {
    template<typename T>
    class modifier;
};

template <typename T>
class arithmetic::modifier
{
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator-(
        const T &lh)
    {
        return T{-value_of(lh)};
    }

    friend
    STRONG_CONSTEXPR
    T&
    operator+=(
        T &lh,
        const T &rh)
    noexcept(noexcept(value_of(lh) += value_of(rh)))
    {
        value_of(lh) += value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    T&
    operator-=(
        T &lh,
        const T &rh)
    noexcept(noexcept(value_of(lh) -= value_of(rh)))
    {
        value_of(lh) -= value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    T&
    operator*=(
        T &lh,
        const T &rh)
    noexcept(noexcept(value_of(lh) *= value_of(rh)))
    {
        value_of(lh) *= value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    T&
    operator/=(
        T &lh,
        const T &rh)
    noexcept(noexcept(value_of(lh) /= value_of(rh)))
    {
        value_of(lh) /= value_of(rh);
        return lh;
    }

    template <typename TT = T, typename = decltype(value_of(std::declval<TT>()) % value_of(std::declval<TT>()))>
    friend
    STRONG_CONSTEXPR
    T&
    operator%=(
        T &lh,
        const T &rh)
    noexcept(noexcept(value_of(lh) %= value_of(rh)))
    {
        value_of(lh) %= value_of(rh);
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator+(
        T lh,
        const T &rh)
    {
        lh += rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator-(
        T lh,
        const T &rh)
    {
        lh -= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator*(
        T lh,
        const T &rh)
    {
        lh *= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator/(
        T lh,
        const T &rh)
    {
        lh /= rh;
        return lh;
    }

    template <typename TT = T, typename = decltype(value_of(std::declval<TT>()) % value_of(std::declval<TT>()))>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator%(
        T lh,
        const T &rh)
    {
        lh %= rh;
        return lh;
    }

};

}

template <typename T, typename Tag, typename ... Ms>
class std::numeric_limits<strong::type<T, Tag, Ms...>>
#if __cplusplus >= 202003L
    requires strong::type_is<strong::type<T, Tag, Ms...>, strong::arithmetic>
        : public std::numeric_limits<T>
#else
    : public std::conditional<
        strong::type_is_v<strong::type<T, Tag, Ms...>, strong::arithmetic>,
        std::numeric_limits<T>,
        std::numeric_limits<void>
    >::type
#endif
{
    using type = strong::type<T, Tag, Ms...>;
public:
    static constexpr type min() noexcept { return type{std::numeric_limits<T>::min()};}
    static constexpr type lowest() noexcept { return type{std::numeric_limits<T>::lowest()};}
    static constexpr type max() noexcept { return type{std::numeric_limits<T>::max()};}
    static constexpr type epsilon() noexcept { return type{std::numeric_limits<T>::epsilon()};}
    static constexpr type round_error() noexcept { return type{std::numeric_limits<T>::round_error()};}
    static constexpr type infinity() noexcept { return type{std::numeric_limits<T>::infinity()};}
    static constexpr type quiet_NaN() noexcept { return type{std::numeric_limits<T>::quiet_NaN()};}
    static constexpr type signaling_NaN() noexcept { return type{std::numeric_limits<T>::signaling_NaN()};}
    static constexpr type denorm_min() noexcept { return type{std::numeric_limits<T>::denorm_min()};}
};
#endif //STRONG_TYPE_ARITHMETIC_HPP
