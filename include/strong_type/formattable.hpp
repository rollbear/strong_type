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

#ifndef STRONG_TYPE_FORMATTABLE_HPP
#define STRONG_TYPE_FORMATTABLE_HPP

#include "type.hpp"

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
#include "ostreamable.hpp"
#include <fmt/format.h>
#include <fmt/ostream.h>

#endif

namespace strong
{

STRONG_TYPE_MODULE_EXPORT struct formattable
{
    template <typename T>
    class modifier{};
};

template <typename T>
using is_formattable = std::is_base_of<formattable::modifier<T>, T>;

}


#if STRONG_HAS_STD_FORMAT

namespace std
{
template<typename T, typename Tag, typename... M, typename Char>
    requires std::is_base_of_v<::strong::formattable::modifier<::strong::type<T, Tag, M...>>,
                               ::strong::type<T, Tag, M...>>
struct formatter<::strong::type<T, Tag, M...>, Char> : formatter<T, Char>
{
  template<typename FormatContext, typename Type>
  STRONG_CONSTEXPR
  decltype(auto)
  format(const Type& t, FormatContext& fc) const
      noexcept(noexcept(std::declval<formatter<T, Char>>().format(std::declval<const T&>(), fc)))
  {
    return formatter<T, Char>::format(value_of(t), fc);
  }

  template<typename FormatContext, typename Type>
  STRONG_CONSTEXPR
  decltype(auto)
  format(const Type& t, FormatContext& fc)
      noexcept(noexcept(std::declval<formatter<T, Char>>().format(std::declval<const T&>(), fc)))
  {
    return formatter<T, Char>::format(value_of(t), fc);
  }
};
}
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
  format(const Type& t, FormatContext& fc) const
      noexcept(noexcept(std::declval<fmt::formatter<T, Char>>().format(std::declval<const T&>(), fc)))
  {
    return fmt::formatter<T, Char>::format(value_of(t), fc);
  }

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

#endif //STRONG_TYPE_FORMATTABLE_HPP
