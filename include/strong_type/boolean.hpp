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

#ifndef STRONG_TYPE_BOOLEAN_HPP
#define STRONG_TYPE_BOOLEAN_HPP

#include "type.hpp"

namespace strong
{
struct boolean
{
    template <typename T>
    class modifier
    {
    public:
        explicit STRONG_CONSTEXPR operator bool() const
        noexcept(noexcept(static_cast<bool>(value_of(std::declval<const T&>()))))
        {
            const auto& self = static_cast<const T&>(*this);
            return static_cast<bool>(value_of(self));
        }
    };
};

}
#endif //STRONG_TYPE_BOOLEAN_HPP
