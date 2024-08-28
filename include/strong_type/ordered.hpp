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

#ifndef STRONG_TYPE_ORDERED_HPP
#define STRONG_TYPE_ORDERED_HPP

#include "type.hpp"

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#if __cpp_impl_three_way_comparison && __has_include(<compare>)
#include <compare>
#endif
#endif

namespace strong
{
struct ordered
{
    template <typename T, typename = void>
    class modifier
    {
        static_assert(impl::always_false<T>,
            "Underlying type must support ordering relations using <, <=, > and >=");
    };
};


template <typename T, typename Tag, typename ... M>
class ordered::modifier<
    ::strong::type<T, Tag, M...>,
        impl::void_t<
            decltype((std::declval<T>() < std::declval<T>())),
            decltype((std::declval<T>() <= std::declval<T>())),
            decltype((std::declval<T>() > std::declval<T>())),
            decltype((std::declval<T>() >= std::declval<T>()))>
>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() < std::declval<const T&>()))
    -> decltype(std::declval<const T&>() < std::declval<const T&>())
    {
        return value_of(lh) < value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() <= std::declval<const T&>()))
    -> decltype(std::declval<const T&>() <= std::declval<const T&>())
    {
        return value_of(lh) <= value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator>(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() > std::declval<const T&>()))
    -> decltype(std::declval<const T&>() > std::declval<const T&>())
    {
        return value_of(lh) > value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR

    auto
    operator>=(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() >= std::declval<const T&>()))
    -> decltype(std::declval<const T&>() >= std::declval<const T&>())
    {
        return value_of(lh) >= value_of(rh);
    }
};

#if __cpp_impl_three_way_comparison && __has_include(<compare>)

namespace detail
{
template<typename Ordering>
struct spaceship_ordering {
    template <typename>
    struct modifier;
};

template<typename Ordering>
template<typename T, typename Tag, typename ... Ms>
struct spaceship_ordering<Ordering>::modifier<::strong::type<T, Tag, Ms...>>
{
    using type = ::strong::type<T, Tag, Ms...>;

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR

    Ordering
    operator<=>(
            const type &lh,
            const type &rh)
    noexcept(noexcept(std::declval<const T &>() <=> std::declval<const T &>()))
    requires std::is_convertible_v<decltype(std::declval<const T&>() <=> std::declval<const T&>()), Ordering>
    {
        return value_of(lh) <=> value_of(rh);
    }
};

}

using strongly_ordered = detail::spaceship_ordering<std::strong_ordering>;
using weakly_ordered = detail::spaceship_ordering<std::weak_ordering>;
using partially_ordered = detail::spaceship_ordering<std::partial_ordering>;

#endif

}

#endif //STRONG_TYPE_ORDERED_HPP
