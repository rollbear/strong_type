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
STRONG_TYPE_MODULE_EXPORT struct bitarithmetic
{
    template <typename T, typename = void>
    class modifier
    {
        static_assert(impl::always_false<T>,
                      "Underlying type must support bitarithmetic operations &, | and ^");
    };
    template <typename T, typename Tag, typename ... Ms>
    class modifier<type<T, Tag, Ms...>, impl::void_t<decltype(((std::declval<T&>() |= std::declval<T>()) &= std::declval<T>()) ^= std::declval<T>())>>
    {
        using type = strong::type<T, Tag, Ms...>;
    public:
        friend
        STRONG_CONSTEXPR
        type&
        operator&=(
            type &lh,
            const type &rh)
        noexcept(noexcept(value_of(lh) &= value_of(rh)))
        {
            value_of(lh) = T(value_of(lh) & value_of(rh));
            return lh;
        }

        friend
        STRONG_CONSTEXPR
        type&
        operator|=(
            type &lh,
            const type &rh)
        noexcept(noexcept(value_of(lh) |= value_of(rh)))
        {
            value_of(lh) = T(value_of(lh) | value_of(rh));
            return lh;
        }

        friend
        STRONG_CONSTEXPR
        type&
        operator^=(
            type &lh,
            const type &rh)
        noexcept(noexcept(value_of(lh) ^= value_of(rh)))
        {
            value_of(lh) = T(value_of(lh) ^ value_of(rh));
            return lh;
        }

        template <typename C>
        friend
        STRONG_CONSTEXPR
        type&
        operator<<=(
            type &lh,
            C c)
        noexcept(noexcept(value_of(lh) <<= c))
        {
            value_of(lh) = T(value_of(lh) << c);
            return lh;
        }

        template <typename C>
        friend
        STRONG_CONSTEXPR
        type&
        operator>>=(
            type &lh,
            C c)
        noexcept(noexcept(value_of(lh) >>= c))
        {
            value_of(lh) = T(value_of(lh) >> c);
            return lh;
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        type
        operator~(
            const type &lh)
        {
            auto v = value_of(lh);
            return type(T(~v));
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        type
        operator&(
            type lh,
            const type &rh)
        {
            lh &= rh;
            return lh;
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        type
        operator|(
            type lh,
            const type &rh)
        {
            lh |= rh;
            return lh;
        }

        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        type
        operator^(
            type lh,
            const type &rh)
        {
            lh ^= rh;
            return lh;
        }

        template <typename C>
        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        type
        operator<<(
            type lh,
            C c)
        {
            lh <<= c;
            return lh;
        }

        template <typename C>
        STRONG_NODISCARD
        friend
        STRONG_CONSTEXPR
        type
        operator>>(
            type lh,
            C c)
        {
            lh >>= c;
            return lh;
        }
    };
};

}
#endif //STRONG_TYPE_BITARITHMETIC_HPP
