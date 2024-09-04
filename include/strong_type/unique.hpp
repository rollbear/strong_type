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

#ifndef STRONG_TYPE_UNIQUE_HPP
#define STRONG_TYPE_UNIQUE_HPP

#include "type.hpp"

namespace strong {

STRONG_TYPE_MODULE_EXPORT struct unique {
    template<typename T>
    class modifier
        : private impl::valid_type<
            impl::require_move_constructible<T>::value &&
            impl::require_move_assignable<T>::value
        > {
    public:
        constexpr modifier() = default;
        modifier(const modifier &) = delete;
        constexpr modifier(modifier &&) noexcept = default;
        modifier &operator=(const modifier &) = delete;
        constexpr modifier &operator=(modifier &&) noexcept = default;
    };
};

}
#endif //STRONG_TYPE_UNIQUE_HPP
