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

#ifndef STRONG_TYPE_SATURATING_HPP
#define STRONG_TYPE_SATURATING_HPP

#include "type.hpp"

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <numeric>
#endif

#if __cpp_lib_saturation_arithmetic

#if __cpp_lib_saturation_arithmetic == 202311L
#define STRONG_SATURATION(op) std::op ## _sat
#else
#define STRONG_SATURATION(op) std::saturating_ ## op
#endif

namespace strong
{
    namespace impl
    {
        template <typename T>
        concept saturating_type = requires (underlying_type_t<T> t){STRONG_SATURATION(add)(t,t);};
    }
struct saturating_arithmetic
{
    template <impl::saturating_type>
    class modifier;
};


template <std::integral T, typename Tag, typename ... M>
    requires requires {STRONG_SATURATION(add)(std::declval<T>(), std::declval<T>()); }
class saturating_arithmetic::modifier<::strong::type<T, Tag, M...>>
{
    using type = strong::type<T, Tag, M...>;
public:
    template <typename U>
    static constexpr void limit(U& u)
    {
#if __cpp_lib_saturation_arithmetic == 202311L
        u = static_cast<U>(std::saturate_cast<T>(u));
#else
        u = static_cast<U>(std::saturating_cast<T>(u));
#endif

    }
    friend
    STRONG_CONSTEXPR
    type& operator+=(type& lh, type rh)
    {
        value_of(lh) = STRONG_SATURATION(add)(value_of(lh), value_of(rh));
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator+(type lh, type rh)
    {
        return lh+= rh;
    }

    friend
    STRONG_CONSTEXPR
    type& operator-=(type& lh, type rh)
    {
        value_of(lh) = STRONG_SATURATION(sub)(value_of(lh), value_of(rh));
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator-(type lh, type rh)
    {
        return lh-= rh;
    }

    friend
    STRONG_CONSTEXPR
    type& operator*=(type& lh, type rh)
    {
        value_of(lh) = STRONG_SATURATION(mul)(value_of(lh), value_of(rh));
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator*(type lh, type rh)
    {
        return lh*= rh;
    }

    friend
    STRONG_CONSTEXPR
    type& operator/=(type& lh, type rh)
    {
        value_of(lh) = STRONG_SATURATION(div)(value_of(lh), value_of(rh));
        return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    type operator/(type lh, type rh)
    {
        return lh/= rh;
    }


};

template <typename T1, typename T2>
    requires strong::type_is_v<T1, saturating_arithmetic>
    && impl::saturating_type<underlying_type_t<T2>>
STRONG_NODISCARD
STRONG_CONSTEXPR
T1 saturating_cast(T2 t2)
    {
#if __cpp_lib_saturation_arithmetic == 202311L
        return T1{std::saturate_cast<underlying_type_t<T1>>(impl::access(t2))};
#else
        return T1{std::saturating_cast<underlying_type_t<T1>>(impl::access(t2))};
#endif
    }


}

#endif
#endif //STRONG_TYPE_SATURATING_HPP
