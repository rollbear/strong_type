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

#ifndef ROLLBEAR_STRONG_TYPE_HPP_INCLUDED
#define ROLLBEAR_STRONG_TYPE_HPP_INCLUDED

#include "type.hpp"
#include "equality.hpp"
#include "equality_with.hpp"
#include "ordered.hpp"
#include "ordered_with.hpp"
#include "semiregular.hpp"
#include "regular.hpp"
#include "unique.hpp"
#include "iostreamable.hpp"
#include "bicrementable.hpp"
#include "boolean.hpp"
#include "pointer.hpp"
#include "hashable.hpp"
#include "difference.hpp"
#include "affine_point.hpp"
#include "arithmetic.hpp"

#include <istream>
#include <ostream>


#ifndef STRONG_HAS_STD_FORMAT
#if __has_include(<version>)
#include <version>
#if defined(__cpp_lib_format) && __cpp_lib_format >= 201907
#define STRONG_HAS_STD_FORMAT 1
#endif
#endif
#endif

#ifndef STRONG_HAS_STD_FORMAT
#define STRONG_HAS_STD_FORMAT 0
#endif

#ifndef STRONG_HAS_FMT_FORMAT
#if __has_include(<fmt/format.h>)
#define STRONG_HAS_FMT_FORMAT 1
#else
#define STRONG_HAS_FMT_FORMAT 0
#endif
#endif

#if STRONG_HAS_STD_FORMAT
#include <format>
#endif

#if STRONG_HAS_FMT_FORMAT
#include <fmt/format.h>
#include <fmt/ostream.h>
#endif

namespace strong
{


struct bitarithmetic
{
  template <typename T>
  class modifier
  {
  public:
    friend
    STRONG_CONSTEXPR
    T&
    operator&=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) &= value_of(rh)))
    {
      using type = underlying_type_t<T>;
      value_of(lh) = type(value_of(lh) & value_of(rh));
      return lh;
    }

    friend
    STRONG_CONSTEXPR
    T&
    operator|=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) |= value_of(rh)))
    {
      using type = underlying_type_t<T>;
      value_of(lh) = type(value_of(lh) | value_of(rh));
      return lh;
    }

    friend
    STRONG_CONSTEXPR
    T&
    operator^=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) ^= value_of(rh)))
    {
      using type = underlying_type_t<T>;
      value_of(lh) = type(value_of(lh) ^ value_of(rh));
      return lh;
    }

    template <typename C>
    friend
    STRONG_CONSTEXPR
    T&
    operator<<=(
      T &lh,
      C c)
    noexcept(noexcept(value_of(lh) <<= c))
    {
      using type = underlying_type_t <T>;
      value_of(lh) = type(value_of(lh) << c);
      return lh;
    }

    template <typename C>
    friend
    STRONG_CONSTEXPR
    T&
    operator>>=(
      T &lh,
      C c)
    noexcept(noexcept(value_of(lh) >>= c))
    {
      using type = underlying_type_t<T>;
      value_of(lh) = type(value_of(lh) >> c);
      return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator~(
      const T &lh)
    {
      using type = underlying_type_t<T>;
      auto v = value_of(lh);
      return T(type(~v));
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator&(
      T lh,
      const T &rh)
    {
      lh &= rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator|(
      T lh,
      const T &rh)
    {
      lh |= rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator^(
      T lh,
      const T &rh)
    {
      lh ^= rh;
      return lh;
    }

    template <typename C>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator<<(
      T lh,
      C c)
    {
      lh <<= c;
      return lh;
    }

    template <typename C>
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    T
    operator>>(
      T lh,
      C c)
    {
      lh >>= c;
      return lh;
    }
  };
};
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

class iterator
{
public:
  template <typename I, typename category = typename std::iterator_traits<underlying_type_t<I>>::iterator_category>
  class modifier
    : public pointer::modifier<I>
    , public equality::modifier<I>
    , public incrementable::modifier<I>
  {
  public:
    using difference_type = typename std::iterator_traits<underlying_type_t<I>>::difference_type;
    using value_type = typename std::iterator_traits<underlying_type_t<I>>::value_type;
    using pointer = typename std::iterator_traits<underlying_type_t<I>>::value_type;
    using reference = typename std::iterator_traits<underlying_type_t<I>>::reference;
    using iterator_category = typename std::iterator_traits<underlying_type_t<I>>::iterator_category;
  };

  template <typename I>
  class modifier<I, std::bidirectional_iterator_tag>
    : public modifier<I, std::forward_iterator_tag>
      , public decrementable::modifier<I>
  {
  };
  template <typename I>
  class modifier<I, std::random_access_iterator_tag>
    : public modifier<I, std::bidirectional_iterator_tag>
      , public affine_point<typename std::iterator_traits<underlying_type_t<I>>::difference_type>::template modifier<I>
      , public indexed<>::modifier<I>
      , public ordered::modifier<I>
  {
  };
};

class range
{
public:
  template <typename R>
  class modifier;
};

template <typename T, typename Tag, typename ... M>
class range::modifier<type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
  using r_iterator = decltype(std::declval<T&>().begin());
  using r_const_iterator = decltype(std::declval<const T&>().begin());
public:
  using iterator = ::strong::type<r_iterator, Tag, strong::iterator>;
  using const_iterator = ::strong::type<r_const_iterator, Tag, strong::iterator>;

  iterator
  begin()
  noexcept(noexcept(std::declval<T&>().begin()))
  {
    auto& self = static_cast<type&>(*this);
    return iterator{value_of(self).begin()};
  }

  iterator
  end()
  noexcept(noexcept(std::declval<T&>().end()))
  {
    auto& self = static_cast<type&>(*this);
    return iterator{value_of(self).end()};
  }

  const_iterator
  cbegin()
    const
  noexcept(noexcept(std::declval<const T&>().begin()))
  {
    auto& self = static_cast<const type&>(*this);
    return const_iterator{value_of(self).begin()};
  }

  const_iterator
  cend()
    const
  noexcept(noexcept(std::declval<const T&>().end()))
  {
    auto& self = static_cast<const type&>(*this);
    return const_iterator{value_of(self).end()};
  }

  const_iterator
  begin()
  const
  noexcept(noexcept(std::declval<const T&>().begin()))
  {
    auto& self = static_cast<const type&>(*this);
    return const_iterator{value_of(self).begin()};
  }

  const_iterator
  end()
  const
  noexcept(noexcept(std::declval<const T&>().end()))
  {
    auto& self = static_cast<const type&>(*this);
    return const_iterator{value_of(self).end()};
  }
};

namespace impl {

  template<typename T, typename D>
  struct converter
  {
    STRONG_CONSTEXPR explicit operator D() const
    noexcept(noexcept(static_cast<D>(std::declval<const underlying_type_t<T>&>())))
    {
      auto& self = static_cast<const T&>(*this);
      return static_cast<D>(value_of(self));
    }
  };
  template<typename T, typename D>
  struct implicit_converter
  {
    STRONG_CONSTEXPR operator D() const
    noexcept(noexcept(static_cast<D>(std::declval<const underlying_type_t<T>&>())))
    {
      auto& self = static_cast<const T&>(*this);
      return static_cast<D>(value_of(self));
    }
  };
}
template <typename ... Ts>
struct convertible_to
{
  template <typename T>
  struct modifier : impl::converter<T, Ts>...
  {
  };
};

template <typename ... Ts>
struct implicitly_convertible_to
{
  template <typename T>
  struct modifier : impl::implicit_converter<T, Ts>...
  {
  };

};

struct formattable
{
  template <typename T>
  class modifier{};
};

template <typename T>
using is_formattable = std::is_base_of<formattable::modifier<T>, T>;

}


#if STRONG_HAS_STD_FORMAT
template<typename T, typename Tag, typename... M, typename Char>
    requires std::is_base_of_v<::strong::formattable::modifier<::strong::type<T, Tag, M...>>,
                               ::strong::type<T, Tag, M...>>
struct formatter<::strong::type<T, Tag, M...>, Char> : formatter<T, Char>
{
  template<typename FormatContext, typename Type>
  STRONG_CONSTEXPR
  decltype(auto)
  format(const Type& t, FormatContext& fc)
      noexcept(noexcept(std::declval<formatter<T, Char>>().format(std::declval<const T&>(), fc)))
  {
    return formatter<T, Char>::format(value_of(t), fc);
  }
};
#endif

#if STRONG_HAS_FMT_FORMAT
namespace strong {

template <typename T, typename Char>
struct formatter;

template <typename T, typename Tag, typename ... M, typename Char>
struct formatter<type<T, Tag, M...>, Char> : fmt::formatter<T, Char>
{
  template<typename FormatContext, typename Type>
  STRONG_CONSTEXPR
  decltype(auto)
  format(const Type& t, FormatContext& fc)
      noexcept(noexcept(std::declval<fmt::formatter<T, Char>>().format(std::declval<const T&>(), fc)))
  {
    return fmt::formatter<T, Char>::format(value_of(t), fc);
  }
};

#if FMT_VERSION >= 90000

template <typename T, typename Char, bool = is_formattable<T>::value>
struct select_formatter;

template <typename T, typename Char>
struct select_formatter<T, Char, true>
{
  using type = formatter<T, Char>;
};

template <typename T, typename Char>
struct select_formatter<T, Char, false>
{
  using type = fmt::ostream_formatter;
};

#endif
}
namespace fmt
{
#if FMT_VERSION >= 90000
template <typename T, typename Tag, typename ... M, typename Char>
struct formatter<::strong::type<T, Tag, M...>,
                 Char,
                 ::strong::impl::void_t<std::enable_if_t<::strong::is_ostreamable<::strong::type<T, Tag, M...>>::value ||
                                                         ::strong::is_formattable<::strong::type<T, Tag, M...>>::value>>>
  :  ::strong::select_formatter<::strong::type<T, Tag, M...>, Char>::type
{
};
#else
template<typename T, typename Tag, typename... M, typename Char>
struct formatter<::strong::type<T, Tag, M...>,
                 Char,
                 ::strong::impl::void_t<std::enable_if_t<::strong::is_formattable<::strong::type<T, Tag, M...>>::value>>
                 >
  :  ::strong::formatter<::strong::type<T, Tag, M...>, Char>
{
};
#endif

}
#endif
#endif //ROLLBEAR_STRONG_TYPE_HPP_INCLUDED
