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

#ifndef STRONG_TYPE_INDEXED_HPP
#define STRONG_TYPE_INDEXED_HPP

#include "type.hpp"

namespace strong
{
template <typename I = void>
struct indexed
{
    template <typename T>
    class modifier;
};

template <>
struct indexed<void> {
    template<typename>
    class modifier;

    template <typename T, typename Tag, typename ... Ms>
    class modifier<type<T, Tag, Ms...>> {
        using ref = T&;
        using cref = const T&;
        using rref = T&&;
        using type = strong::type<T, Tag, Ms...>;
    public:
        template<typename I>
        STRONG_NODISCARD
        auto
        operator[](
            const I &i)
        const &
        noexcept(noexcept(std::declval<cref>()[impl::access(i)]))
        -> decltype(std::declval<cref>()[impl::access(i)]) {
            auto& self = static_cast<const type&>(*this);
            return value_of(self)[impl::access(i)];
        }

        template<typename I>
        STRONG_NODISCARD
        auto
        operator[](
            const I &i)
        &
        noexcept(noexcept(std::declval<ref>()[impl::access(i)]))
        -> decltype(std::declval<ref>()[impl::access(i)]) {
            auto& self = static_cast<type&>(*this);
            return value_of(self)[impl::access(i)];
        }

        template<typename I>
        STRONG_NODISCARD
        auto
        operator[](
            const I &i)
        &&
        noexcept(noexcept(std::declval<rref>()[impl::access(i)]))
        -> decltype(std::declval<rref>()[impl::access(i)]) {
            auto& self = static_cast<type&>(*this);
            return value_of(std::move(self))[impl::access(i)];
        }

        template<typename I, typename C = cref>
        STRONG_NODISCARD
        auto
        at(
            const I &i)
        const &
        -> decltype(std::declval<C>().at(impl::access(i))) {
            auto& self = static_cast<const type&>(*this);
            return value_of(self).at(impl::access(i));
        }

        template<typename I, typename R = ref>
        STRONG_NODISCARD
        auto
        at(
            const I &i)
        &
        -> decltype(std::declval<R>().at(impl::access(i))) {
            auto& self = static_cast<type&>(*this);
            return value_of(self).at(impl::access(i));
        }

        template<typename I, typename R = rref>
        STRONG_NODISCARD
        auto
        at(
            const I &i)
        &&
        -> decltype(std::declval<R>().at(impl::access(i))) {
            auto& self = static_cast<type&>(*this);
            return value_of(std::move(self)).at(impl::access(i));
        }
    };
};

template <typename I>
template <typename T, typename Tag, typename ... M>
class indexed<I>::modifier<type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    STRONG_NODISCARD
    auto
    operator[](
        const I& i)
    const &
    noexcept(noexcept(std::declval<const T&>()[impl::access(i)]))
    -> decltype(std::declval<const T&>()[impl::access(i)])
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self)[impl::access(i)];
    }

    STRONG_NODISCARD
    auto
    operator[](
        const I& i)
    &
    noexcept(noexcept(std::declval<T&>()[impl::access(i)]))
    -> decltype(std::declval<T&>()[impl::access(i)])
    {
        auto& self = static_cast<type&>(*this);
        return value_of(self)[impl::access(i)];
    }

    STRONG_NODISCARD
    auto
    operator[](
        const I& i)
    &&
    noexcept(noexcept(std::declval<T&&>()[impl::access(i)]))
    -> decltype(std::declval<T&&>()[impl::access(i)])
    {
        auto& self = static_cast<type&>(*this);
        return value_of(std::move(self))[impl::access(i)];
    }

    template <typename TT = T>
    STRONG_NODISCARD
    auto
    at(
        const I& i)
    const &
    -> decltype(std::declval<const TT&>().at(impl::access(i)))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).at(impl::access(i));
    }

    template <typename TT = T>
    STRONG_NODISCARD
    auto
    at(
        const I& i)
    &
    -> decltype(std::declval<TT&>().at(impl::access(i)))
    {
        auto& self = static_cast<type&>(*this);
        return value_of(self).at(impl::access(i));
    }

    template <typename TT = T>
    STRONG_NODISCARD
    auto
    at(
        const I& i)
    &&
    -> decltype(std::declval<TT&&>().at(impl::access(i)))
    {
        auto& self = static_cast<type&>(*this);
        return value_of(std::move(self)).at(impl::access(i));
    }
};

}
#endif //STRONG_TYPE_INDEXED_HPP
