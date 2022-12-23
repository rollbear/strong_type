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

#ifndef STRONG_TYPE_BITARITHMETIC_HPP
#define STRONG_TYPE_BITARITHMETIC_HPP

#include "type.hpp"

namespace strong
{
struct bitarithmetic
{
    template <typename T>
    class modifier
    {
    public:
        friend
        STRONG_CONSTEXPR
        T&
        operator&=(
            T &lh,
            const T &rh)
        noexcept(noexcept(value_of(lh) &= value_of(rh)))
        {
            using type = underlying_type_t<T>;
            value_of(lh) = type(value_of(lh) & value_of(rh));
            return lh;
        }

        friend
        STRONG_CONSTEXPR
        T&
        operator|=(
            T &lh,
            const T &rh)
        noexcept(noexcept(value_of(lh) |= value_of(rh)))
        {
            using type = underlying_type_t<T>;
            value_of(lh) = type(value_of(lh) | value_of(rh));
            return lh;
        }

        friend
        STRONG_CONSTEXPR
        T&
        operator^=(
            T &lh,
            const T &rh)
        noexcept(noexcept(value_of(lh) ^= value_of(rh)))
        {
            using type = underlying_type_t<T>;
            value_of(lh) = type(value_of(lh) ^ value_of(rh));
            return lh;
        }

        template <typename C>
        friend
        STRONG_CONSTEXPR
        T&
        operator<<=(
            T &lh,
            C c)
        noexcept(noexcept(value_of(lh) <<= c))
        {
            using type = underlying_type_t <T>;
            value_of(lh) = type(value_of(lh) << c);
            return lh;
        }

        template <typename C>
        friend
        STRONG_CONSTEXPR
        T&
        operator>>=(
            T &lh,
            C c)
        noexcept(noexcept(value_of(lh) >>= c))
        {
            using type = underlying_type_t<T>;
            value_of(lh) = type(value_of(lh) >> c);
            return lh;
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        T
        operator~(
            const T &lh)
        {
            using type = underlying_type_t<T>;
            auto v = value_of(lh);
            return T(type(~v));
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        T
        operator&(
            T lh,
            const T &rh)
        {
            lh &= rh;
            return lh;
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        T
        operator|(
            T lh,
            const T &rh)
        {
            lh |= rh;
            return lh;
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        T
        operator^(
            T lh,
            const T &rh)
        {
            lh ^= rh;
            return lh;
        }

        template <typename C>
        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        T
        operator<<(
            T lh,
            C c)
        {
            lh <<= c;
            return lh;
        }

        template <typename C>
        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        T
        operator>>(
            T lh,
            C c)
        {
            lh >>= c;
            return lh;
        }
    };
};

}
#endif //STRONG_TYPE_BITARITHMETIC_HPP
