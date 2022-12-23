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

#ifndef STRONG_TYPE_HASHABLE_HPP
#define STRONG_TYPE_HASHABLE_HPP

#include "type.hpp"

#include <functional>

namespace strong
{
struct hashable
{
    template <typename T>
    class modifier{};
};

}

namespace std {
template<typename T, typename Tag, typename ... M>
struct hash<::strong::type<T, Tag, M...>>
    : std::conditional_t<
        std::is_base_of<
            ::strong::hashable::modifier<
                ::strong::type<T, Tag, M...>
            >,
            ::strong::type<T, Tag, M...>
        >::value,
        hash<T>,
        std::false_type>
{
    using type = ::strong::type<T, Tag, M...>;

    decltype(auto)
    operator()(
        const ::strong::hashable::modifier<type> &t)
    const
    noexcept(noexcept(std::declval<hash<T>>()(
        value_of(std::declval<const type &>()))))
    {
        auto &tt = static_cast<const type &>(t);
        return hash<T>::operator()(value_of(tt));
    }
};
}
#endif //STRONG_TYPE_HASHABLE_HPP
