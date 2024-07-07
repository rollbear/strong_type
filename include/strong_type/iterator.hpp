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

#ifndef STRONG_TYPE_ITERATOR_HPP
#define STRONG_TYPE_ITERATOR_HPP

#include "pointer.hpp"
#include "bicrementable.hpp"
#include "affine_point.hpp"
#include "indexed.hpp"

#include <iterator>

#if __cplusplus >= 202002L && (((! defined(_LIBCPP_VERSION) || _LIBCPP_VERSION >= 15000)) && (!defined(__GLIBCXX__) || __GLIBCXX__ >= 20230528L))
#define STRONG_TYPE_CONTIGUOUS_ITERATOR 1
#else
#define STRONG_TYPE_CONTIGUOUS_ITERATOR 0
#endif

namespace strong
{
namespace internal {
    template <typename I, typename = void>
    struct iterator_traits
    {
        using iterator_category = std::input_iterator_tag;
        using difference_type = typename I::difference_type;
        using reference = decltype(*std::declval<I>());
        using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;
        using pointer = value_type*;
    };
    template <typename I>
    struct iterator_traits<I, impl::void_t<typename I::iterator_category>>
    {
        using iterator_category = typename I::iterator_category;
        using difference_type = typename I::difference_type;
        using reference = decltype(*std::declval<I>());
        using value_type = std::remove_cv_t<std::remove_reference_t<reference>>;
        using pointer = value_type*;

    };
    template <typename T>
    struct iterator_traits<T*, void>
    {
#if STRONG_TYPE_CONTIGUOUS_ITERATOR
        using iterator_category = std::contiguous_iterator_tag;
#else
        using iterator_category = std::random_access_iterator_tag;
#endif
        using difference_type = ptrdiff_t;
        using value_type = T;
        using reference = T&;
        using pointer = T*;

    };
}
class iterator
{
public:
    template <typename I,
            typename category = typename internal::iterator_traits<underlying_type_t<I>>::iterator_category>
    class modifier
        : public pointer::modifier<I>
        , public incrementable::modifier<I>
    {
    public:
        using difference_type = typename internal::iterator_traits<underlying_type_t<I>>::difference_type;
        using value_type = typename internal::iterator_traits<underlying_type_t<I>>::value_type;
        using pointer = typename internal::iterator_traits<underlying_type_t<I>>::pointer;
        using reference = typename internal::iterator_traits<underlying_type_t<I>>::reference;
        using iterator_category = typename internal::iterator_traits<underlying_type_t<I>>::iterator_category;
    };

    template <typename I>
    class modifier<I, std::forward_iterator_tag>
        : public modifier<I, std::input_iterator_tag>
        , public strong::equality::modifier<I>
    {};
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

#if STRONG_TYPE_CONTIGUOUS_ITERATOR
    template <typename I>
    class modifier<I, std::contiguous_iterator_tag>
            : public modifier<I, std::bidirectional_iterator_tag>
                    , public affine_point<typename std::iterator_traits<underlying_type_t<I>>::difference_type>::template modifier<I>
                    , public indexed<>::modifier<I>
                    , public ordered::modifier<I>
    {
    };
#endif
};

}
#endif //STRONG_TYPE_ITERATOR_HPP