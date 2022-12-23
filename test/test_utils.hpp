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

#ifndef STRONG_TYPE_TEST_UTILS_HPP
#define STRONG_TYPE_TEST_UTILS_HPP

#include <utility>

template <typename T>
const T& as_const(T& t) { return t;}
template <typename T>
const T&& as_const(T&& t) { return std::move(t);}

#endif //STRONG_TYPE_TEST_UTILS_HPP
