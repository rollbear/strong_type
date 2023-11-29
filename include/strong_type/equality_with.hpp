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

#ifndef STRONG_TYPE_EQUALITY_WITH_HPP
#define STRONG_TYPE_EQUALITY_WITH_HPP

#include "type.hpp"

namespace strong
{
namespace impl
{
template <typename T, typename Other, typename = void>
class typed_equality {
    static_assert(impl::always_false<T, Other>,
                  "Underlying type must be equality comparable with target type");
};

template <typename T, typename Other>
class typed_equality<
    T,
    Other,
    impl::void_t<decltype(std::declval<const underlying_type_t<T>&>() == std::declval<const underlying_type_t<Other>&>())>
>
{
private:
    using TT = underlying_type_t<T>;
    using OT = underlying_type_t<Other>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator==(const T& lh, const Other& rh)
    noexcept(noexcept(std::declval<const TT&>() == std::declval<const OT&>()))
    -> decltype(std::declval<const TT&>() == std::declval<const OT&>())
    {
        return value_of(lh) == impl::access(rh);
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator==(const Other& lh, const T& rh)
    noexcept(noexcept(std::declval<const OT&>() == std::declval<const TT&>()))
    -> decltype(std::declval<const OT&>() == std::declval<const TT&>())
    {
        return impl::access(lh) == value_of(rh) ;
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator!=(const T& lh, const Other rh)
    noexcept(noexcept(std::declval<const TT&>() != std::declval<const OT&>()))
    -> decltype(std::declval<const TT&>() != std::declval<const OT&>())
    {
        return value_of(lh) != impl::access(rh);
    }
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto operator!=(const Other& lh, const T& rh)
    noexcept(noexcept(std::declval<const OT&>() != std::declval<const TT&>()))
    -> decltype(std::declval<const OT&>() != std::declval<const TT&>())
    {
        return impl::access(lh) != value_of(rh) ;
    }
};
}

template <typename ... Ts>
struct equality_with
{
    template <typename T>
    class modifier : public impl::typed_equality<T, Ts>...
    {
    };
};

}
#endif //STRONG_TYPE_EQUALITY_WITH_HPP
