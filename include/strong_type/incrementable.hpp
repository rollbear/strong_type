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

#ifndef STRONG_TYPE_INCREMENTABLE_HPP
#define STRONG_TYPE_INCREMENTABLE_HPP

#include "type.hpp"

namespace strong {
STRONG_TYPE_MODULE_EXPORT struct incrementable
{
    template <typename T, typename = void>
    class modifier
    {
        static_assert(impl::always_false<T>,
                      "Underlying type must be incrementable");
    };
    template <typename T>
    class modifier<T, impl::void_t<decltype(++std::declval<underlying_type_t<T>&>())>>
    {
    public:
        friend
        STRONG_CONSTEXPR
        T&
        operator++(T& t)
        noexcept(noexcept(++std::declval<T&>().value_of()))
        {
            ++value_of(t);
            return t;
        }

        friend
        STRONG_CONSTEXPR
        T
        operator++(T& t, int)
        {
            auto copy = t;
            ++t;
            return copy;
        }
    };
};

}
#endif //STRONG_TYPE_INCREMENTABLE_HPP
