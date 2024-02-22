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

#include "iterator.hpp"

namespace strong
{
class range
{
public:
    template <typename R, typename = void>
    class modifier
    {
        static_assert(impl::always_false<R>,
                      "Underlying type must have members .begin() and .end()");
    };
};

template <typename T, typename Tag, typename ... M>
class range::modifier<
    type<T, Tag, M...>,
    impl::void_t<
        decltype(std::declval<T>().begin() == std::declval<T>().end())
    >
>
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

    template <typename TT = const T&>
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
