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

#ifndef STRONG_TYPE_RANGE_HPP
#define STRONG_TYPE_RANGE_HPP

#if __has_include(<ranges>)
#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#    include <ranges>
#endif
#endif
#if defined(_MSC_VER) && __cpp_lib_ranges >= 202110L
#    define STRONG_TYPE_HAS_RANGES
#endif
#if __cplusplus >= 202101L
#    if (not defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L)) && (__cpp_lib_ranges >= 202106L)
#        define STRONG_TYPE_HAS_RANGES
#    endif
#    if (not defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 15000)
#        define STRONG_TYPE_HAS_RANGES
#    endif
#endif
#include "iterator.hpp"
#include "equality_with.hpp"
#include "ordered_with.hpp"

namespace strong
{

namespace internal {
struct not_an_iterator {};


#if defined(STRONG_TYPE_HAS_RANGES) && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L))
template <typename T>
auto begin_type(T* t) -> decltype(std::ranges::begin(*t));
#else
template <typename T>
auto begin_type(T* t) -> decltype(t->begin());
#endif

auto begin_type(...) -> not_an_iterator;

#if defined(STRONG_TYPE_HAS_RANGES) && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L))
template <typename T>
auto const_begin_type(const T* t) -> decltype(std::ranges::begin(*t));
#else
template <typename T>
auto const_begin_type(const T* t) -> decltype(t->begin());
#endif
auto const_begin_type(...) -> not_an_iterator;

#if defined(STRONG_TYPE_HAS_RANGES) && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L))
template <typename T>
auto end_type(T* t) -> decltype(std::ranges::end(*t));
#else
template <typename T>
auto end_type(T* t) -> decltype(t->end());
#endif
auto end_type(...) -> not_an_iterator;

#if defined(STRONG_TYPE_HAS_RANGES) && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L))
template <typename T>
auto const_end_type(const T* t) -> decltype(std::ranges::end(*t));
#else
template <typename T>
auto const_end_type(const T* t) -> decltype(t->end());
#endif

auto const_end_type(...) -> not_an_iterator;

#if defined(STRONG_TYPE_HAS_RANGES) && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L))
template <typename T>
auto cbegin_type(const T* t) -> decltype(std::ranges::cbegin(*t));
#else
template <typename T>
auto cbegin_type(const T* t) -> decltype(t->cbegin());
#endif
auto cbegin_type(...) -> not_an_iterator;

#if defined(STRONG_TYPE_HAS_RANGES) && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230528L))
template <typename T>
auto cend_type(const T* t) -> decltype(std::ranges::cend(*t));
#else
template <typename T>
auto cend_type(const T* t) -> decltype(t->cend());
#endif
auto cend_type(...) -> not_an_iterator;

template <typename T>
struct begin_end_traits
{
  using begin_iterator = decltype(begin_type(std::declval<T*>()));
  using const_begin_iterator = decltype(const_begin_type(std::declval<const T*>()));
  using end_iterator = decltype(end_type(std::declval<T*>()));
  using const_end_iterator = decltype(const_end_type(std::declval<const T*>()));
  using cbegin_iterator = decltype(cbegin_type(std::declval<const T*>()));
  using cend_iterator = decltype(cend_type(std::declval<const T*>()));
};

constexpr bool is_random_access(std::random_access_iterator_tag) { return true;}
constexpr bool is_random_access(std::input_iterator_tag) { return false;}
constexpr bool is_random_access(std::output_iterator_tag) { return false;}
}

#if defined(STRONG_TYPE_HAS_RANGES)  && (!defined(__GLIBCXX__) || (__GLIBCXX__ >= 20230601L))
#define STRONG_TYPE_BEGIN(x) std::ranges::begin(x)
#define STRONG_TYPE_END(x) std::ranges::end(x)
#define STRONG_TYPE_CBEGIN(x) std::ranges::cbegin(x)
#define STRONG_TYPE_CEND(x) std::ranges::cend(x)
#else
#define STRONG_TYPE_BEGIN(x) x.begin()
#define STRONG_TYPE_END(x) x.end()
#define STRONG_TYPE_CBEGIN(x) x.cbegin()
#define STRONG_TYPE_CEND(x) x.cend()
#endif

class range
{
public:
    template <
            typename R,
            typename = typename internal::begin_end_traits<underlying_type_t<R>>::begin_iterator,
            typename = typename internal::begin_end_traits<underlying_type_t<R>>::end_iterator,
            typename = typename internal::begin_end_traits<underlying_type_t<R>>::const_begin_iterator,
            typename = typename internal::begin_end_traits<underlying_type_t<R>>::const_end_iterator,
            typename = typename internal::begin_end_traits<underlying_type_t<R>>::cbegin_iterator,
            typename = typename internal::begin_end_traits<underlying_type_t<R>>::cend_iterator>
    class modifier;
};

template <typename R>
class range::modifier<
        R,
        internal::not_an_iterator, internal::not_an_iterator,
        internal::not_an_iterator, internal::not_an_iterator,
        internal::not_an_iterator, internal::not_an_iterator
>
{
    static_assert(impl::always_false<R>,
                  "Underlying type must have begin() and end()");

};

template <typename T, typename Tag, typename ... M, typename r_iterator>
class range::modifier<
        type<T, Tag, M...>,
        r_iterator, r_iterator,
        r_iterator, r_iterator,
        r_iterator, r_iterator>
{
    using type = ::strong::type<T, Tag, M...>;
    static constexpr bool random_access = internal::is_random_access(typename internal::iterator_traits<r_iterator>::iterator_category{});
public:
    using const_iterator = std::conditional_t<random_access,
            ::strong::type<r_iterator, Tag, strong::iterator, strong::default_constructible, strong::equality_with<r_iterator>, strong::ordered_with<r_iterator>>,
            ::strong::type<r_iterator, Tag, strong::iterator, strong::default_constructible, strong::equality_with<r_iterator>>
    >;

    STRONG_NODISCARD
    constexpr
    const_iterator
    begin()
    const
    noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    end()
    const
    noexcept(noexcept(STRONG_TYPE_END(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_END(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    cbegin()
    const
    noexcept(noexcept(STRONG_TYPE_CBEGIN(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_CBEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    cend()
    const
    noexcept(noexcept(STRONG_TYPE_CEND(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_CEND(value_of(self))};
    }

    template <typename TT = const T&>
    STRONG_NODISCARD
    constexpr
    decltype(std::declval<TT>().size())
    size()
    const
    noexcept(noexcept(std::declval<TT>().size()))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).size();
    }

};
template <typename T, typename Tag, typename ... M, typename r_iterator, typename r_const_iterator>
class range::modifier<
    type<T, Tag, M...>,
    r_iterator, r_iterator,
    r_const_iterator, r_const_iterator,
    r_const_iterator, r_const_iterator
>
{
    using type = ::strong::type<T, Tag, M...>;
    static constexpr bool random_access = internal::is_random_access(typename internal::iterator_traits<r_iterator>::iterator_category{});
public:
    using iterator = ::strong::type<r_iterator, Tag, strong::iterator, strong::default_constructible>;
    using const_iterator = std::conditional_t<random_access,
            ::strong::type<r_const_iterator, Tag, strong::iterator, strong::default_constructible, strong::equality_with<iterator>, strong::ordered_with<iterator>>,
            ::strong::type<r_const_iterator, Tag, strong::iterator, strong::default_constructible, strong::equality_with<iterator>>
    >;

    STRONG_NODISCARD
    constexpr
    iterator
    begin()
    noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    iterator
    end()
    noexcept(noexcept(STRONG_TYPE_END(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return iterator{STRONG_TYPE_END(value_of(self))};
    }


    STRONG_NODISCARD
    constexpr
    const_iterator
    begin()
    const
    noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    end()
    const
    noexcept(noexcept(STRONG_TYPE_END(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_END(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    cbegin()
    const
    noexcept(noexcept(STRONG_TYPE_CBEGIN(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_CBEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    cend()
    const
    noexcept(noexcept(STRONG_TYPE_CEND(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_CEND(value_of(self))};
    }

    template <typename TT = const T&>
    STRONG_NODISCARD
    constexpr
    decltype(std::declval<TT>().size())
    size()
    const
    noexcept(noexcept(std::declval<TT>().size()))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).size();
    }
};

template <typename T, typename Tag, typename ... M, typename r_iterator, typename r_const_iterator, typename r_sentinel>
class range::modifier<
        type<T, Tag, M...>,
        r_iterator, r_sentinel,
        r_iterator, r_sentinel,
        r_const_iterator, r_sentinel
>
{
    using type = ::strong::type<T, Tag, M...>;
    static constexpr bool random_access = internal::is_random_access(typename internal::iterator_traits<r_iterator>::iterator_category{});
public:
    using iterator = ::strong::type<r_iterator, Tag, strong::iterator, strong::default_constructible>;
    using const_iterator = std::conditional_t<random_access,
            ::strong::type<r_const_iterator, Tag, strong::iterator, strong::default_constructible, strong::equality_with<iterator>, strong::ordered_with<iterator>>,
            ::strong::type<r_const_iterator, Tag, strong::iterator, strong::default_constructible, strong::equality_with<iterator>>
    >;
    using sentinel = strong::type<r_sentinel, Tag, strong::equality_with<iterator, const_iterator>>;

    STRONG_NODISCARD
    constexpr
    iterator
    begin()
        noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    sentinel
    end()
        noexcept(noexcept(STRONG_TYPE_END(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return sentinel{STRONG_TYPE_END(value_of(self))};
    }


    STRONG_NODISCARD
    constexpr
    iterator
    begin()
    const
        noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    sentinel
    end()
    const
        noexcept(noexcept(STRONG_TYPE_END(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return sentinel{STRONG_TYPE_END(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    const_iterator
    cbegin()
    const
        noexcept(noexcept(STRONG_TYPE_CBEGIN(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_CBEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    sentinel
    cend()
    const
        noexcept(noexcept(STRONG_TYPE_CEND(std::declval<const T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return sentinel{STRONG_TYPE_CEND(value_of(self))};
    }

    template <typename TT = const T&>
    STRONG_NODISCARD
    constexpr
    decltype(std::declval<TT>().size())
    size()
    const
        noexcept(noexcept(std::declval<TT>().size()))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).size();
    }
};

template <typename T, typename Tag, typename ... M, typename r_iterator>
class range::modifier<
            type<T, Tag, M...>,
            r_iterator, r_iterator,
            internal::not_an_iterator, internal::not_an_iterator,
            internal::not_an_iterator, internal::not_an_iterator
>
{
    using type = ::strong::type<T, Tag, M...>;
    public:
    using iterator = ::strong::type<r_iterator, Tag, strong::iterator, strong::default_constructible>;

    STRONG_NODISCARD
    constexpr
    iterator
    begin()
        noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    iterator
    end()
        noexcept(noexcept(STRONG_TYPE_END(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return iterator{STRONG_TYPE_END(value_of(self))};
    }

    template <typename TT = const T&>
    STRONG_NODISCARD
    constexpr
    decltype(std::declval<TT>().size())
    size()
    const
    noexcept(noexcept(std::declval<TT>().size()))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).size();
    }
};

template <typename T, typename Tag, typename ... M, typename r_iterator, typename r_sentinel>
class range::modifier<
        type<T, Tag, M...>,
        r_iterator, r_sentinel,
        internal::not_an_iterator, internal::not_an_iterator,
        internal::not_an_iterator, internal::not_an_iterator
>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    using iterator = ::strong::type<r_iterator, Tag, strong::iterator, strong::default_constructible>;
    using sentinel = ::strong::type<r_sentinel, Tag, strong::equality_with<iterator>>;

    STRONG_NODISCARD
    constexpr
    iterator
    begin()
        noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    sentinel
    end()
        noexcept(noexcept(STRONG_TYPE_END(std::declval<T&>())))
    {
        auto& self = static_cast<type&>(*this);
        return sentinel{STRONG_TYPE_END(value_of(self))};
    }

    template <typename TT = const T&>
    STRONG_NODISCARD
    constexpr
    decltype(std::declval<TT>().size())
    size()
    const
        noexcept(noexcept(std::declval<TT>().size()))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).size();
    }
};

template <typename T, typename Tag, typename ... M, typename r_const_iterator, typename r_sentinel>
class range::modifier<
        type<T, Tag, M...>,
        internal::not_an_iterator, internal::not_an_iterator,
        r_const_iterator, r_sentinel,
        internal::not_an_iterator, internal::not_an_iterator
>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    using const_iterator = ::strong::type<r_const_iterator, Tag, strong::iterator, strong::default_constructible>;
    using sentinel = ::strong::type<r_sentinel, Tag, strong::equality_with<const_iterator>>;

    STRONG_NODISCARD
    constexpr
    const_iterator
    begin()
    const
        noexcept(noexcept(STRONG_TYPE_BEGIN(std::declval<T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return const_iterator{STRONG_TYPE_BEGIN(value_of(self))};
    }

    STRONG_NODISCARD
    constexpr
    sentinel
    end()
    const
        noexcept(noexcept(STRONG_TYPE_END(std::declval<T&>())))
    {
        auto& self = static_cast<const type&>(*this);
        return sentinel{STRONG_TYPE_END(value_of(self))};
    }

    template <typename TT = const T&>
    STRONG_NODISCARD
    constexpr
    decltype(std::declval<TT>().size())
    size()
    const
        noexcept(noexcept(std::declval<TT>().size()))
    {
        auto& self = static_cast<const type&>(*this);
        return value_of(self).size();
    }
};

}
#endif //STRONG_TYPE_RANGE_HPP


