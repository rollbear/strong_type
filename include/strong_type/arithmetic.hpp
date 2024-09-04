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
STRONG_TYPE_MODULE_EXPORT struct arithmetic {
    template<typename T, typename = void>
    class modifier
    {
        static_assert(impl::always_false<T>,
                      "Underlying type must support arithmeric operations");
    };
};

STRONG_TYPE_MODULE_EXPORT template <typename T, typename Tag, typename ... Ms>
class arithmetic::modifier<strong::type<T, Tag, Ms...>, impl::void_t<decltype(std::declval<T>() * std::declval<T>() - std::declval<T>() / std::declval<T>())>>
{
    using type = strong::type<T, Tag, Ms...>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator-(
        const type &lh)
    {
        return type{-value_of(lh)};
    }

    friend
    STRONG_CONSTEXPR
    type&
    operator+=(
        type &lh,
        const type &rh)
    noexcept(noexcept(value_of(lh) += value_of(rh)))
    {
        value_of(lh) += value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type&
    operator-=(
        type &lh,
        const type &rh)
    noexcept(noexcept(value_of(lh) -= value_of(rh)))
    {
        value_of(lh) -= value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type&
    operator*=(
        type &lh,
        const type &rh)
    noexcept(noexcept(value_of(lh) *= value_of(rh)))
    {
        value_of(lh) *= value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type&
    operator/=(
        type &lh,
        const type &rh)
    noexcept(noexcept(value_of(lh) /= value_of(rh)))
    {
        value_of(lh) /= value_of(rh);
        return lh;
    }

    template <typename TT = T, typename = decltype(std::declval<TT>() % std::declval<TT>())>
    friend
    STRONG_CONSTEXPR
    type&
    operator%=(
        type &lh,
        const type &rh)
    noexcept(noexcept(value_of(lh) %= value_of(rh)))
    {
        value_of(lh) %= value_of(rh);
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator+(
        type lh,
        const type &rh)
    {
        lh += rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator-(
        type lh,
        const type &rh)
    {
        lh -= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator*(
        type lh,
        const type &rh)
    {
        lh *= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator/(
        type lh,
        const type &rh)
    {
        lh /= rh;
        return lh;
    }

    template <typename TT = T, typename = decltype(std::declval<TT>() % std::declval<TT>())>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator%(
        type lh,
        const type &rh)
    {
        lh %= rh;
        return lh;
    }

};

}

STRONG_TYPE_MODULE_EXPORT template <typename T, typename Tag, typename ... Ms>
#if defined(__cpp_concepts)
requires strong::type_is_v<strong::type<T, Tag, Ms...>, strong::arithmetic>
class std::numeric_limits<strong::type<T, Tag, Ms...>>
    : public std::numeric_limits<T>
#else
class std::numeric_limits<strong::type<T, Tag, Ms...>>
    : public std::conditional<
        strong::type_is_v<strong::type<T, Tag, Ms...>, strong::arithmetic>,
        std::numeric_limits<T>,
        std::numeric_limits<void>
    >::type
#endif
{
    using type = strong::type<T, Tag, Ms...>;
public:
    STRONG_NODISCARD static constexpr type min() noexcept { return type{std::numeric_limits<T>::min()};}
    STRONG_NODISCARD static constexpr type lowest() noexcept { return type{std::numeric_limits<T>::lowest()};}
    STRONG_NODISCARD static constexpr type max() noexcept { return type{std::numeric_limits<T>::max()};}
    STRONG_NODISCARD static constexpr type epsilon() noexcept { return type{std::numeric_limits<T>::epsilon()};}
    STRONG_NODISCARD static constexpr type round_error() noexcept { return type{std::numeric_limits<T>::round_error()};}
    STRONG_NODISCARD static constexpr type infinity() noexcept { return type{std::numeric_limits<T>::infinity()};}
    STRONG_NODISCARD static constexpr type quiet_NaN() noexcept { return type{std::numeric_limits<T>::quiet_NaN()};}
    STRONG_NODISCARD static constexpr type signaling_NaN() noexcept { return type{std::numeric_limits<T>::signaling_NaN()};}
    STRONG_NODISCARD static constexpr type denorm_min() noexcept { return type{std::numeric_limits<T>::denorm_min()};}
};
#endif //STRONG_TYPE_ARITHMETIC_HPP
