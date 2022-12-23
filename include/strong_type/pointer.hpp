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

#ifndef STRONG_TYPE_POINTER_HPP
#define STRONG_TYPE_POINTER_HPP

#include "type.hpp"

namespace strong
{
struct pointer
{
    template <typename T>
    class modifier;
};

template <typename T, typename Tag, typename ... M>
class pointer::modifier<::strong::type<T, Tag, M...>>
{
    using type = strong::type<T, Tag, M...>;
public:
    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator==(
        const type& t,
        std::nullptr_t)
    noexcept(noexcept(std::declval<const TT&>() == nullptr))
    -> decltype(std::declval<const TT&>() == nullptr)
    {
        return value_of(t) == nullptr;
    }

    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator==(
        std::nullptr_t,
        const type& t)
    noexcept(noexcept(nullptr == std::declval<const TT&>()))
    -> decltype(nullptr == std::declval<const TT&>())
    {
        return value_of(t) == nullptr;
    }

    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator!=(
        const type& t,
        std::nullptr_t)
    noexcept(noexcept(std::declval<const TT&>() != nullptr))
    -> decltype(std::declval<const TT&>() != nullptr)
    {
        return value_of(t) != nullptr;
    }

    template <typename TT = T>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    auto
    operator!=(
        std::nullptr_t,
        const type& t)
    noexcept(noexcept(nullptr != std::declval<const TT&>()))
    -> decltype(nullptr != std::declval<const TT&>())
    {
        return value_of(t) != nullptr;
    }

    STRONG_NODISCARD
    STRONG_CONSTEXPR
    decltype(*std::declval<const T&>())
    operator*()
    const
    {
        auto& self = static_cast<const type&>(*this);
        return *value_of(self);
    }

    STRONG_NODISCARD
    STRONG_CONSTEXPR
    decltype(&(*std::declval<const T&>())) operator->() const { return &operator*();}
};

}
#endif //STRONG_TYPE_POINTER_HPP
