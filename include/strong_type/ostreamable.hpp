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

#ifndef STRONG_TYPE_OSTREAMABLE_HPP
#define STRONG_TYPE_OSTREAMABLE_HPP

#include "type.hpp"

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <ostream>
#endif

namespace strong
{
struct ostreamable
{
    template <typename T, typename = void>
    class modifier
    {
        static_assert(impl::always_false<T>,
            "Underlying type support stream output via operator<<");
    };
    template <typename T>
    class modifier<T, impl::void_t<decltype(std::declval<std::ostream&>() << std::declval<const underlying_type_t<T>&>())>>
    {
    public:
        friend
        std::ostream&
        operator<<(
            std::ostream &os,
            const T &t)
        {
            return os << value_of(t);
        }
    };
};

template<typename T>
using is_ostreamable = std::is_base_of<ostreamable::modifier<T>, T>;

}
#endif //STRONG_TYPE_OSTREAMABLE_HPP
