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

#ifndef ROLLBEAR_STRONG_TYPE_TYPE_HPP_INCLUDED
#define ROLLBEAR_STRONG_TYPE_TYPE_HPP_INCLUDED


#include <type_traits>
#include <initializer_list>
#include <utility>

#if defined(_MSC_VER) && !defined(__clang__) && _MSC_VER < 1922
#define STRONG_CONSTEXPR
#else
#define STRONG_CONSTEXPR constexpr
#endif

#if __cplusplus >= 201703L
#define STRONG_NODISCARD [[nodiscard]]
#else
#define STRONG_NODISCARD
#endif

namespace strong {
struct uninitialized_t {
};
static constexpr uninitialized_t uninitialized{};

template<typename M, typename T>
using modifier = typename M::template modifier<T>;


struct default_constructible {
    template<typename T>
    class modifier {
    };
};

namespace impl {
template<typename T>
constexpr bool supports_default_construction(
    const ::strong::default_constructible::modifier<T> *)
{
    return true;
}

template<typename T, typename ... V>
using WhenConstructible = std::enable_if_t<std::is_constructible<T, V...>::value>;
}

template<typename T, typename Tag, typename ... M>
class type : public modifier<M, type<T, Tag, M...>> ... {
public:
    template<typename TT = T, typename = std::enable_if_t<std::is_trivially_constructible<TT>{}>>
    explicit type(uninitialized_t)
    noexcept
    {
    }

    template<typename type_ = type,
        bool = impl::supports_default_construction(
            static_cast<type_ *>(nullptr))>
    constexpr
    type()
    noexcept(noexcept(T{}))
        : val{}
    {
    }

    template<typename U,
        typename = impl::WhenConstructible<T, std::initializer_list<U>>>
    constexpr
    explicit
    type(
        std::initializer_list<U> us
    )
    noexcept(noexcept(T{us}))
        : val{us}
    {
    }

    template<typename ... U,
        typename = std::enable_if_t<
            std::is_constructible<T, U &&...>::value && (sizeof...(U) > 0)>>
    constexpr
    explicit
    type(
        U &&... u)
    noexcept(std::is_nothrow_constructible<T, U...>::value)
        : val(std::forward<U>(u)...)
    {}

    friend STRONG_CONSTEXPR void swap(type &a, type &b) noexcept(
    std::is_nothrow_move_constructible<T>::value &&
    std::is_nothrow_move_assignable<T>::value
    )
    {
        using std::swap;
        swap(a.val, b.val);
    }

    STRONG_NODISCARD
    constexpr T &value_of() & noexcept
    { return val; }

    STRONG_NODISCARD
    constexpr const T &value_of() const & noexcept
    { return val; }

    STRONG_NODISCARD
    constexpr T &&value_of() && noexcept
    { return std::move(val); }

    STRONG_NODISCARD
    friend constexpr T &value_of(type &t) noexcept
    { return t.val; }

    STRONG_NODISCARD
    friend constexpr const T &value_of(const type &t) noexcept
    { return t.val; }

    STRONG_NODISCARD
    friend constexpr T &&value_of(type &&t) noexcept
    { return std::move(t).val; }

private:
    T val;
};

namespace impl {
template<typename T, typename Tag, typename ... Ms>
constexpr bool is_strong_type_func(const strong::type<T, Tag, Ms...> *)
{ return true; }

constexpr bool is_strong_type_func(...)
{ return false; }

template<typename T, typename Tag, typename ... Ms>
constexpr T underlying_type(strong::type<T, Tag, Ms...> *);

}

template<typename T>
struct is_strong_type : std::integral_constant<bool, impl::is_strong_type_func(
    static_cast<T *>(nullptr))> {
};


template<typename T, bool = is_strong_type<T>::value>
struct underlying_type {
    using type = decltype(impl::underlying_type(static_cast<T *>(nullptr)));
};

template<typename T>
struct underlying_type<T, false> {
    using type = T;
};

template<typename T>
using underlying_type_t = typename underlying_type<T>::type;

namespace impl {
template<typename T>
using WhenStrongType = std::enable_if_t<is_strong_type<std::decay_t<T>>::value>;
template<typename T>
using WhenNotStrongType = std::enable_if_t<!is_strong_type<std::decay_t<T>>::value>;

template<
    typename T,
    typename = impl::WhenNotStrongType<T>>
constexpr
T &&
access(T &&t)
noexcept
{
    return std::forward<T>(t);
}

template<
    typename T,
    typename = impl::WhenStrongType<T>>
STRONG_NODISCARD
constexpr
auto
access(T &&t)
noexcept
-> decltype(value_of(std::forward<T>(t)))
{
    return value_of(std::forward<T>(t));
}

}
}
#endif //ROLLBEAR_STRONG_TYPE_TYPE_HPP_INCLUDED
