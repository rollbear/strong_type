#ifndef STRONG_TYPE_STRONG_ORDERING_HPP
#define STRONG_TYPE_STRONG_ORDERING_HPP

#include <compare>

#include "type.hpp"

namespace strong
{
STRONG_TYPE_MODULE_EXPORT struct strong_ordering
{
    template <typename T>
    class modifier;
};

STRONG_TYPE_MODULE_EXPORT template <typename T, typename Tag, typename ... M>
class strong_ordering::modifier<::strong::type<T, Tag, M...>>
{
    using type = ::strong::type<T, Tag, M...>;
public:
    STRONG_NODISCARD
    friend
    STRONG_CONSTEXPR
    std::strong_ordering operator<=>(const type& lh, const type& rh) noexcept
    {
      return value_of(lh) <=> value_of(rh);
    }
};
}

#endif //STRONG_TYPE_STRONG_ORDERING_HPP
