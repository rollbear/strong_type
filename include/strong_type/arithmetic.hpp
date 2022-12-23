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
#endif //STRONG_TYPE_ARITHMETIC_HPP
