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

#ifndef STRONG_TYPE_ISTREAMABLE_HPP
#define STRONG_TYPE_ISTREAMABLE_HPP

#include "type.hpp"

#include <istream>

namespace strong
{
STRONG_TYPE_MODULE_EXPORT struct istreamable
{
    template <typename T, typename = void>
    class  modifier
    {
        static_assert(impl::always_false<T>,
            "Underlying type must support stream input via operator>>");
    };
    template <typename T>
    class modifier<T, impl::void_t<decltype(std::declval<std::istream&>() >> std::declval<underlying_type_t<T>&>())>>
    {
    public:
        friend
        std::istream&
        operator>>(
            std::istream &is,
            T &t)
        {
            return is >> value_of(t);
        }
    };
};

}
#endif //STRONG_TYPE_ISTREAMABLE_HPP
