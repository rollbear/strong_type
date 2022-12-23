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

#ifndef STRONG_TYPE_AFFINE_POINT_HPP
#define STRONG_TYPE_AFFINE_POINT_HPP

#include "difference.hpp"

namespace strong
{
template <typename D = void>
struct affine_point
{
    template <typename T>
    class modifier;
};

namespace impl
{
template <typename T, typename = void>
struct subtractable : std::false_type {};

template <typename T>
struct subtractable<T, void_t<decltype(std::declval<const T&>() - std::declval<const T&>())>>
    : std::true_type {};
}


template <typename D>
template <typename T, typename Tag, typename ... M>
class affine_point<D>::modifier<::strong::type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
    static_assert(impl::subtractable<T>::value, "it must be possible to subtract instances of your underlying type");
    using base_diff_type = decltype(std::declval<const T&>() - std::declval<const T&>());
public:
    using difference = std::conditional_t<std::is_same<D, void>{}, strong::type<base_diff_type, Tag, strong::difference>, D>;
    static_assert(std::is_constructible<difference, base_diff_type>::value, "");
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    difference
    operator-(
        const type& lh,
        const type& rh)
    {
        return difference(value_of(lh) - value_of(rh));
    }

    friend
    STRONG_CONSTEXPR
    type&
    operator+=(
        type& lh,
        const difference& d)
    noexcept(noexcept(value_of(lh) += impl::access(d)))
    {
        value_of(lh) += impl::access(d);
        return lh;
    }

    friend
    STRONG_CONSTEXPR
    type&
    operator-=(
        type& lh,
        const difference& d)
    noexcept(noexcept(value_of(lh) -= impl::access(d)))
    {
        value_of(lh) -= impl::access(d);
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator+(
        type lh,
        const difference& d)
    {
        return lh += d;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator+(
        const difference& d,
        type rh)
    {
        return rh+= d;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type
    operator-(
        type lh,
        const difference& d)
    {
        return lh -= d;
    }
};

}
#endif //STRONG_TYPE_AFFINE_POINT_HPP
