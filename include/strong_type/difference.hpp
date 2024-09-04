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

#ifndef STRONG_TYPE_DIFFERENCE_HPP
#define STRONG_TYPE_DIFFERENCE_HPP

#include "ordered.hpp"
#include "equality.hpp"

namespace strong
{
namespace impl
{
struct conditionally_ordered
{
    template <typename T>
    class modifier;
};
template <typename T, typename Tag, typename ... M>
class conditionally_ordered::modifier<::strong::type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const TT&>() < std::declval<const TT&>()))
    -> decltype(std::declval<const TT&>() < std::declval<const TT&>())
    {
        return value_of(lh) < value_of(rh);
    }
    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const TT&>() <= std::declval<const TT&>()))
    -> decltype(std::declval<const TT&>() <= std::declval<const TT&>())
    {
        return value_of(lh) <= value_of(rh);
    }
    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator>(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const TT&>() > std::declval<const TT&>()))
    -> decltype(std::declval<const TT&>() > std::declval<const TT&>())
    {
        return value_of(lh) > value_of(rh);
    }
    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator>=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const TT&>() >= std::declval<const TT&>()))
    -> decltype(std::declval<const TT&>() >= std::declval<const TT&>())
    {
        return value_of(lh) >= value_of(rh);
    }
};
}
STRONG_TYPE_MODULE_EXPORT struct difference
{
    template <typename T>
    class modifier;
};

STRONG_TYPE_MODULE_EXPORT template <typename T, typename Tag, typename ... M>
class difference::modifier<::strong::type<T, Tag, M...>>
    : public impl::conditionally_ordered::modifier<::strong::type<T, Tag, M...>>
    , public equality::modifier<::strong::type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    friend
    STRONG_CONSTEXPR
    type& operator+=(type& lh, const type& rh)
    noexcept(noexcept(value_of(lh) += value_of(rh)))
    {
        value_of(lh) += value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type& operator-=(type& lh, const type& rh)
    noexcept(noexcept(value_of(lh) -= value_of(rh)))
    {
        value_of(lh) -= value_of(rh);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type& operator*=(type& lh, const T& rh)
    noexcept(noexcept(value_of(lh) *= rh))
    {
        value_of(lh) *= rh;
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type& operator/=(type& lh, const T& rh)
    noexcept(noexcept(value_of(lh) /= rh))
    {
        value_of(lh) /= rh;
        return lh;
    }

    template <typename TT = T, typename = decltype(std::declval<TT&>()%= std::declval<const TT&>())>
    friend
    STRONG_CONSTEXPR
    type& operator%=(type& lh, const T& rh)
    noexcept(noexcept(value_of(lh) %= rh))
    {
        value_of(lh)%= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator+(type lh, const type& rh)
    {
        lh += rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator-(type lh, const type& rh)
    {
        lh -= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator*(type lh, const T& rh)
    {
        lh *= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator*(const T& lh, type rh)
    {
        rh *= lh;
        return rh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator/(type lh, const T& rh)
    {
        lh /= rh;
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T operator/(const type& lh, const type& rh)
    {
        return value_of(lh) / value_of(rh);
    }

    template <typename TT = T, typename = decltype(std::declval<TT&>() %= std::declval<const TT&>())>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator%(type lh, const T& rh)
    noexcept(noexcept(lh%= rh))
    {
        lh %= rh;
        return lh;
    }

    template <typename TT = T, typename = decltype(std::declval<TT>() % std::declval<TT>())>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T operator%(type lh, type rh)
    noexcept(noexcept(value_of(lh) % value_of(rh)))
    {
        return value_of(lh) % value_of(rh);
    }
};

}
#endif //STRONG_TYPE_DIFFERENCE_HPP
