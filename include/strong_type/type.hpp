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

// For users including rather than importing the header we don't want to force them to define STRONG_TYPE_MODULE_EXPORT so if not defined  define it to be empty
#if !defined(STRONG_TYPE_MODULE_EXPORT)
#define STRONG_TYPE_MODULE_EXPORT
#endif

namespace strong {
STRONG_TYPE_MODULE_EXPORT struct uninitialized_t {
};
STRONG_TYPE_MODULE_EXPORT constexpr uninitialized_t uninitialized{};

STRONG_TYPE_MODULE_EXPORT template<typename M, typename T>
using modifier = typename M::template modifier<T>;


STRONG_TYPE_MODULE_EXPORT struct default_constructible {
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

STRONG_TYPE_MODULE_EXPORT template<typename T, typename Tag, typename ... M>
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
        : _val{}
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
        : _val{us}
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
        : _val(std::forward<U>(u)...)
    {}

    friend STRONG_CONSTEXPR void swap(type &a, type &b) noexcept(
    std::is_nothrow_move_constructible<T>::value &&
    std::is_nothrow_move_assignable<T>::value
    )
    {
        using std::swap;
        swap(a._val, b._val);
    }

    STRONG_NODISCARD
    constexpr T &value_of() & noexcept
    { return _val; }

    STRONG_NODISCARD
    constexpr const T &value_of() const & noexcept
    { return _val; }

    STRONG_NODISCARD
    constexpr T &&value_of() && noexcept
    { return std::move(_val); }

    STRONG_NODISCARD
    constexpr const T &&value_of() const && noexcept
    { return std::move(_val); }

    STRONG_NODISCARD
    friend constexpr T &value_of(type &t) noexcept
    { return t._val; }

    STRONG_NODISCARD
    friend constexpr const T &value_of(const type &t) noexcept
    { return t._val; }

    STRONG_NODISCARD
    friend constexpr T &&value_of(type &&t) noexcept
    { return std::move(t)._val; }

    STRONG_NODISCARD
    friend constexpr const T &&value_of(const type &&t) noexcept
    { return std::move(t)._val; }

    T _val;
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

STRONG_TYPE_MODULE_EXPORT template<typename T>
struct is_strong_type : std::integral_constant<bool, impl::is_strong_type_func(
    static_cast<T *>(nullptr))> {
};


STRONG_TYPE_MODULE_EXPORT template<typename T, bool = is_strong_type<T>::value>
struct underlying_type {
    using type = decltype(impl::underlying_type(static_cast<T *>(nullptr)));
};

STRONG_TYPE_MODULE_EXPORT template<typename T>
struct underlying_type<T, false> {
    using type = T;
};

STRONG_TYPE_MODULE_EXPORT template<typename T>
using underlying_type_t = typename underlying_type<T>::type;

namespace impl {
template<typename T>
using WhenStrongType = std::enable_if_t<is_strong_type<std::decay_t<T>>::value>;
template<typename T>
using WhenNotStrongType = std::enable_if_t<!is_strong_type<std::decay_t<T>>::value>;

// This is used in the implementation of public functions so must be exported
STRONG_TYPE_MODULE_EXPORT template<
    typename T,
    typename = impl::WhenNotStrongType<T>>
constexpr
T &&
access(T &&t)
noexcept
{
    return std::forward<T>(t);
}

// This is used in the implementation of public functions so must be exported
STRONG_TYPE_MODULE_EXPORT template<
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

template <typename T>
struct require_copy_constructible
{
    static constexpr bool value = std::is_copy_constructible<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be copy constructible");
};
template <typename T>
struct require_move_constructible
{
    static constexpr bool value = std::is_move_constructible<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be move constructible");
};
template <typename T>
struct require_copy_assignable
{
    static constexpr bool value = std::is_copy_assignable<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be copy assignable");
};
template <typename T>
struct require_move_assignable
{
    static constexpr bool value = std::is_move_assignable<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be move assignable");
};

template <typename ...>
static constexpr bool always_false = false;

template <bool> struct valid_type;
template <>
struct valid_type<true> {};

template<typename ...>
using void_t = void;


template <typename Needle, typename Type, typename Modifier>
static constexpr bool modifier_is
= std::is_base_of<modifier<Needle, Type>, modifier<Modifier,Type>>::value;

template <typename Needle, typename Type, typename Haystack>
static
constexpr
bool
    type_implements
    = std::is_base_of<modifier<Needle, Type>, modifier<Haystack, Type>>::value;

#if __cplusplus >= 201703
template <
    template <typename...> class Modifier,
    typename Needle,
    typename Type,
    typename ... Ms
>
static
constexpr
bool
modifier_is<Needle, Type, Modifier<Ms...>>
= (std::is_base_of<modifier<Needle, Type>, modifier<Modifier<Ms>, Type>>::value || ...);

template <
    template <typename ...> class Modifier,
    typename ... Needles,
    typename Type,
    typename ... Haystack
>
static
constexpr
bool type_implements<Modifier<Needles...>, Type, Modifier<Haystack...>>
= (modifier_is<Modifier<Needles>, Type, Modifier<Haystack...>> && ...);
#else
template <typename ...>
struct conjunction;
template <typename T, typename ... Ts>
struct conjunction<T, Ts...>
    : std::integral_constant<bool, T::value && conjunction<Ts...>::value>
    {};
template <>
struct conjunction<> : std::true_type {};

template <typename ...>
struct disjunction;
template <typename T, typename ... Ts>
struct disjunction<T, Ts...>
    : std::integral_constant<bool, T::value || disjunction<Ts...>::value>
    {};
template <>
struct disjunction<> : std::false_type {};

template <
    template <typename...> class Modifier,
    typename Needle,
    typename Type,
    typename ... Ms
>
static
constexpr
bool
modifier_is<Needle, Type, Modifier<Ms...>>
    = disjunction<
        std::is_base_of<
            modifier<Needle, Type>,
            modifier<Modifier<Ms>, Type>
        >...
    >::value;

template <
    template <typename ...> class Modifier,
    typename ... Needles,
    typename Type,
    typename ... Haystack
>
static
constexpr
bool
type_implements<Modifier<Needles...>, Type, Modifier<Haystack...>>
    = conjunction<
        std::integral_constant<
            bool,
            modifier_is<
                Modifier<Needles>,
                Type,
                Modifier<Haystack...>
            >
        >...
    >::value;
#endif

template <typename T, typename M>
static constexpr bool type_is = false;

#if __cplusplus >= 201703L
template <typename T, typename Tag, typename ... Ms, typename M>
static constexpr bool type_is<strong::type<T, Tag, Ms...>, M>
    = (impl::type_implements<M, strong::type<T, Tag, Ms...>, Ms> || ...);
#else
template <typename T, typename Tag, typename ... Ms, typename M>
static constexpr bool type_is<strong::type<T, Tag, Ms...>, M>
    = impl::disjunction<std::integral_constant<bool, impl::type_implements<M, strong::type<T, Tag, Ms...>, Ms>>...>::value;
#endif

template <typename T, typename Tag, typename ... Ms>
type<T, Tag, Ms...> get_strong_(const type<T, Tag, Ms...>*);

template <typename T>
using get_strong = decltype(get_strong_(static_cast<T*>(nullptr)));
}

STRONG_TYPE_MODULE_EXPORT template <typename T, typename M>
constexpr bool type_is_v = impl::type_is<impl::get_strong<T>, M>;

STRONG_TYPE_MODULE_EXPORT template <typename T, typename M>
using type_is = std::integral_constant<bool, type_is_v<T,M>>;

}

#endif //ROLLBEAR_STRONG_TYPE_TYPE_HPP_INCLUDED
