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

namespace strong
{
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

}
#endif //STRONG_TYPE_ITERATOR_HPP