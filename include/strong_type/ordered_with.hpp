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

#ifndef STRONG_TYPE_ORDERED_WITH_HPP
#define STRONG_TYPE_ORDERED_WITH_HPP

#include "type.hpp"

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#if __cpp_impl_three_way_comparison && __has_include(<compare>)
#include <compare>
#endif
#endif

namespace strong
{
namespace impl
{
template <typename T, typename Other, typename = void>
class typed_ordering
{
    static_assert(impl::always_false<T, Other>,
                  "Underlying type must support ordering relations with target type using operators <, <=, > and >=");
};

template <typename T, typename Other>
class typed_ordering<
    T,
    Other,
    impl::void_t<
        decltype((std::declval<underlying_type_t<T>>() < std::declval<underlying_type_t<Other>>())),
        decltype((std::declval<underlying_type_t<T>>() <= std::declval<underlying_type_t<Other>>())),
        decltype((std::declval<underlying_type_t<T>>() > std::declval<underlying_type_t<Other>>())),
        decltype((std::declval<underlying_type_t<T>>() >= std::declval<underlying_type_t<Other>>()))
    >
>
{
private:
    using TT = underlying_type_t<T>;
    using OT = underlying_type_t<Other>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator<(const T& lh, const Other& rh)
    noexcept(noexcept(std::declval<const TT&>() < std::declval<const OT&>()))
    -> decltype(std::declval<const TT&>() < std::declval<const OT&>())
    {
        return value_of(lh) < impl::access(rh);
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator<(const Other& lh, const T& rh)
    noexcept(noexcept(std::declval<const OT&>() < std::declval<const TT&>()))
    -> decltype(std::declval<const OT&>() < std::declval<const TT&>())
    {
        return impl::access(lh) < value_of(rh) ;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator<=(const T& lh, const Other& rh)
    noexcept(noexcept(std::declval<const TT&>() <= std::declval<const OT&>()))
    -> decltype(std::declval<const TT&>() <= std::declval<const OT&>())
    {
        return value_of(lh) <= impl::access(rh);
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator<=(const Other& lh, const T& rh)
    noexcept(noexcept(std::declval<const OT&>() <= std::declval<const TT&>()))
    -> decltype(std::declval<const OT&>() <= std::declval<const TT&>())
    {
        return impl::access(lh) <= value_of(rh) ;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator>(const T& lh, const Other& rh)
    noexcept(noexcept(std::declval<const TT&>() > std::declval<const OT&>()))
    -> decltype(std::declval<const TT&>() > std::declval<const OT&>())
    {
        return value_of(lh) > impl::access(rh);
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator>(const Other& lh, const T& rh)
    noexcept(noexcept(std::declval<const OT&>() > std::declval<const TT&>()))
    -> decltype(std::declval<const OT&>() > std::declval<const TT&>())
    {
        return impl::access(lh) > value_of(rh) ;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator>=(const T& lh, const Other& rh)
    noexcept(noexcept(std::declval<const TT&>() >= std::declval<const OT&>()))
    -> decltype(std::declval<const TT&>() >= std::declval<const OT&>())
    {
        return value_of(lh) >= impl::access(rh);
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator>=(const Other& lh, const T& rh)
    noexcept(noexcept(std::declval<const OT&>() >= std::declval<const TT&>()))
    -> decltype(std::declval<const OT&>() >= std::declval<const TT&>())
    {
        return impl::access(lh) >= value_of(rh) ;
    }
};
}

template <typename ... Ts>
struct ordered_with
{
    template <typename T>
    class modifier : public impl::typed_ordering<T, Ts>...
    {
    };
};

#if __cpp_impl_three_way_comparison && __has_include(<compare>)

namespace detail
{


template <typename Ordering, typename T, typename Other>
struct typed_spaceship_ordering_with
{
private:
    using TT = underlying_type_t<T>;
    using OT = underlying_type_t<Other>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    Ordering
    operator<=>(
        const T& lh,
        const Other& rh)
    noexcept(noexcept(std::declval<const TT&>() <=> std::declval<const OT&>()))
    requires std::is_convertible_v<decltype(std::declval<const TT&>() <=> std::declval<const OT&>()), Ordering>
    {
        return value_of(lh) <=> impl::access(rh);
    }
};
template <typename Ordering, typename ... Ts>
struct spaceship_ordering_with
{
    template <typename T>
    struct modifier : public typed_spaceship_ordering_with<Ordering, T, Ts>...
    {
    };
};

}

template <typename ... Ts>
using strongly_ordered_with = detail::spaceship_ordering_with<std::strong_ordering, Ts...>;
template <typename ... Ts>
using weakly_ordered_with = detail::spaceship_ordering_with<std::weak_ordering, Ts...>;
template <typename ... Ts>
using partially_ordered_with = detail::spaceship_ordering_with<std::partial_ordering, Ts...>;

#endif

}
#endif //STRONG_TYPE_ORDERED_WITH_HPP
