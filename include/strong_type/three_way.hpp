/*
 * strong_type C++14/17/20 strong typedef library
 *
 * Copyright (C) Björn Schäpers
 *
 *  Use, modification and distribution is subject to the
 *  Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at
 *  http://www.boost.org/LICENSE_1_0.txt)
 *
 * Project home: https://github.com/rollbear/strong_type
 */

#ifndef STRONG_TYPE_THREE_WAY_HPP
#define STRONG_TYPE_THREE_WAY_HPP

#if __cplusplus >= 202002L
#include <version>
#if defined(__cpp_lib_three_way_comparison) && __cpp_lib_three_way_comparison >= 201907
#define STRONG_HAS_COMPARE 1

#include "type.hpp"

#include <compare>

namespace strong
{
struct three_way
{
    template <typename T>
    class modifier;
};


template <typename T, typename Tag, typename ... M>
class three_way::modifier<::strong::type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator<=>(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() <=> std::declval<const T&>()))
    -> decltype(std::declval<const T&>() <=> std::declval<const T&>())
    {
        return value_of(lh) <=> value_of(rh);
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator==(
        const type& lh,
        const type& rh)
    noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()))
    -> decltype(std::declval<const T&>() == std::declval<const T&>())
    {
        return value_of(lh) == value_of(rh);
    }
};

}
#endif //defined(__cpp_lib_three_way_comparison) && __cpp_lib_three_way_comparison >= 201907
#endif //__cplusplus >= 202002L

#ifndef STRONG_HAS_COMPARE
#define STRONG_HAS_COMPARE 0
#endif //STRONG_HAS_COMPARE
#endif //STRONG_TYPE_THREE_WAY_HPP
