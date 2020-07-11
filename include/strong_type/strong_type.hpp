#ifndef SAFE_TYPE_SAFE_TYPE_HPP
#define SAFE_TYPE_SAFE_TYPE_HPP

#include <functional>
#include <istream>
#include <ostream>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201703L
#define STRONG_NODISCARD [[nodiscard]]
#else
#define STRONG_NODISCARD
#endif

namespace strong
{

namespace impl
{
  template <typename T, typename ... V>
  using WhenConstructible = std::enable_if_t<std::is_constructible<T, V...>::value>;
}

template <typename M, typename T>
using modifier = typename M::template modifier<T>;

struct uninitialized_t {};
static constexpr uninitialized_t uninitialized{};

struct default_constructible
{
  template <typename T>
  class modifier
  {
  };
};

namespace impl {
  template <typename T>
  constexpr bool supports_default_construction(const ::strong::default_constructible::modifier<T>*)
  {
    return true;
  }
}

template <typename T, typename Tag, typename ... M>
class type : public modifier<M, type<T, Tag, M...>>...
{
public:
  template <typename TT = T, typename = std::enable_if_t<std::is_trivially_constructible<TT>{}>>
  explicit type(uninitialized_t)
    noexcept
  {
  }
  template <typename type_ = type,
            bool = impl::supports_default_construction(static_cast<type_*>(nullptr))>
  constexpr
  type()
    noexcept(noexcept(T{}))
  : val{}
  {
  }

  template <typename U,
    typename = impl::WhenConstructible<T, std::initializer_list<U>>>
  constexpr
  explicit
  type(
    std::initializer_list<U> us
  )
    noexcept(noexcept(T{us}))
  : val{us}
  {
  }
  template <typename ... U,
            typename = std::enable_if_t<std::is_constructible<T, U&&...>::value && (sizeof...(U) > 0)>>
  constexpr
  explicit
  type(
    U&& ... u)
  noexcept(std::is_nothrow_constructible<T, U...>::value)
  : val(std::forward<U>(u)...)
  {}

  friend void swap(type& a, type& b) noexcept(
                                        std::is_nothrow_move_constructible<type>::value &&
                                        std::is_nothrow_move_assignable<type>::value
                                      )
  {
    using std::swap;
    swap(a.val, b.val);
  }

  STRONG_NODISCARD
  constexpr T& value_of() & noexcept { return val;}
  STRONG_NODISCARD
  constexpr const T& value_of() const & noexcept { return val;}
  STRONG_NODISCARD
  constexpr T&& value_of() && noexcept { return std::move(val);}

  STRONG_NODISCARD
  friend constexpr T& value_of(type& t) noexcept { return t.val;}
  STRONG_NODISCARD
  friend constexpr const T& value_of(const type& t) noexcept { return t.val;}
  STRONG_NODISCARD
  friend constexpr T&& value_of(type&& t) noexcept { return std::move(t).val;}
private:
  T val;
};

template <typename T>
struct is_safe_type : std::false_type {};
template <typename T, typename Tag, typename ... M>
struct is_safe_type<type<T, Tag, M...>> : std::true_type {};

namespace impl {
  template <typename T>
  using WhenSafeType = std::enable_if_t<is_safe_type<std::decay_t<T>>::value>;
  template <typename T>
  using WhenNotSafeType = std::enable_if_t<!is_safe_type<std::decay_t<T>>::value>;
}

template <typename T>
struct underlying_type
{
  using type = T;
};

template <typename T, typename Tag, typename ... M>
struct underlying_type<type<T, Tag, M...>>
{
  using type = T;
};

template <typename T>
using underlying_type_t = typename underlying_type<T>::type;


namespace impl {
  template<
    typename T,
    typename = impl::WhenNotSafeType<T>>
  constexpr
  T &&
  access(T &&t)
  noexcept {
    return std::forward<T>(t);
  }
  template <
    typename T,
    typename = impl::WhenSafeType<T>>
  STRONG_NODISCARD
  constexpr
  auto
  access(T&& t)
  noexcept
  -> decltype(value_of(std::forward<T>(t)))
  {
    return value_of(std::forward<T>(t));
  }

}
struct equality
{
  template <typename T>
  class modifier;
};


template <typename T, typename Tag, typename ... M>
class equality::modifier<::strong::type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
public:
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator==(
    const type& lh,
    const type& rh)
  noexcept(noexcept(std::declval<const T&>() == std::declval<const T&>()))
  -> decltype(std::declval<const T&>() == std::declval<const T&>())
  {
    return value_of(lh) == value_of(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator!=(
    const type& lh,
    const type& rh)
  noexcept(noexcept(std::declval<const T&>() != std::declval<const T&>()))
  -> decltype(std::declval<const T&>() != std::declval<const T&>())
  {
    return value_of(lh) != value_of(rh);
  }
};

namespace impl
{
  template <typename T>
  struct require_copy_constructible
  {
    static constexpr bool value = std::is_copy_constructible<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be copy constructible");
  };
  template <typename T>
  struct require_move_constructible
  {
    static constexpr bool value = std::is_move_constructible<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be move constructible");
  };
  template <typename T>
  struct require_copy_assignable
  {
    static constexpr bool value = std::is_copy_assignable<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be copy assignable");
  };
  template <typename T>
  struct require_move_assignable
  {
    static constexpr bool value = std::is_move_assignable<underlying_type_t<T>>::value;
    static_assert(value, "underlying type must be move assignable");
  };

  template <bool> struct valid_type;
  template <>
  struct valid_type<true> {};

  template <typename T>
  struct require_semiregular
    : valid_type<require_copy_constructible<T>::value &&
                 require_move_constructible<T>::value &&
                 require_copy_assignable<T>::value &&
                 require_move_assignable<T>::value>
  {
  };

}
struct semiregular
{
  template <typename>
  class modifier;
};

template <typename T, typename Tag, typename ... M>
class semiregular::modifier<::strong::type<T, Tag, M...>>
  : public default_constructible::modifier<T>
  , private impl::require_semiregular<T>
{
};

struct regular
{
  template <typename T>
  class modifier
    : public semiregular::modifier<T>
    , public equality::modifier<T>
  {
  };
};

struct ordered
{
  template <typename T>
  class modifier;
};


template <typename T, typename Tag, typename ... M>
class ordered::modifier<::strong::type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
public:
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator<(
    const type& lh,
    const type& rh)
  noexcept(noexcept(std::declval<const T&>() < std::declval<const T&>()))
  -> decltype(std::declval<const T&>() < std::declval<const T&>())
  {
    return value_of(lh) < value_of(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator<=(
    const type& lh,
    const type& rh)
  noexcept(noexcept(std::declval<const T&>() <= std::declval<const T&>()))
  -> decltype(std::declval<const T&>() <= std::declval<const T&>())
  {
    return value_of(lh) <= value_of(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator>(
    const type& lh,
    const type& rh)
  noexcept(noexcept(std::declval<const T&>() > std::declval<const T&>()))
  -> decltype(std::declval<const T&>() > std::declval<const T&>())
  {
    return value_of(lh) > value_of(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator>=(
    const type& lh,
    const type& rh)
  noexcept(noexcept(std::declval<const T&>() >= std::declval<const T&>()))
  -> decltype(std::declval<const T&>() >= std::declval<const T&>())
  {
    return value_of(lh) >= value_of(rh);
  }
};

struct ostreamable
{
  template <typename T>
  class modifier
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

struct istreamable
{
  template <typename T>
  class modifier
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

struct iostreamable
{
  template <typename T>
  class modifier
    : public ostreamable::modifier<T>
    , public istreamable::modifier<T>
  {
  };
};

struct incrementable
{
  template <typename T>
  class modifier
  {
  public:
    constexpr
    T&
    operator++()
    noexcept(noexcept(++std::declval<T&>().value_of()))
    {
      auto &self = static_cast<T&>(*this);
      ++value_of(self);
      return self;
    }

    constexpr
    T
    operator++(int)
    {
      auto copy = static_cast<T&>(*this);
      ++*this;
      return copy;
    }
  };
};

struct decrementable
{
  template <typename T>
  class modifier
  {
  public:
    constexpr
    T&
    operator--()
    noexcept(noexcept(--std::declval<T&>().value_of()))
    {
      auto &self = static_cast<T&>(*this);
      --value_of(self);
      return self;
    }

    constexpr
    T
    operator--(int)
    {
      auto copy = static_cast<T&>(*this);
      --*this;
      return copy;
    }
  };
};

struct bicrementable
{
  template <typename T>
  class modifier
    : public incrementable::modifier<T>
    , public decrementable::modifier<T>
  {
  };
};

struct boolean
{
  template <typename T>
  class modifier
  {
  public:
    explicit constexpr operator bool() const
    noexcept(noexcept(static_cast<bool>(value_of(std::declval<const T&>()))))
    {
      const auto& self = static_cast<const T&>(*this);
      return static_cast<bool>(value_of(self));
    }
  };
};

struct hashable
{
  template <typename T>
  class modifier{};
};

struct difference
{
  template <typename T>
  class modifier;
};

template <typename T, typename Tag, typename ... M>
class difference::modifier<::strong::type<T, Tag, M...>>
: public ordered::modifier<::strong::type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
public:
  friend type& operator+=(type& lh, const type& rh)
  noexcept(noexcept(value_of(lh) += value_of(rh)))
  {
    value_of(lh) += value_of(rh);
    return lh;
  }

  friend type& operator-=(type& lh, const type& rh)
    noexcept(noexcept(value_of(lh) -= value_of(rh)))
  {
    value_of(lh) -= value_of(rh);
    return lh;
  }

  friend type& operator*=(type& lh, const T& rh)
  noexcept(noexcept(value_of(lh) *= rh))
  {
    value_of(lh) *= rh;
    return lh;
  }

  friend type& operator/=(type& lh, const T& rh)
    noexcept(noexcept(value_of(lh) /= rh))
  {
    value_of(lh) /= rh;
    return lh;
  }

  friend
  type operator+(type lh, const type& rh)
  {
    lh += rh;
    return lh;
  }

  friend
  type operator-(type lh, const type& rh)
  {
    lh -= rh;
    return lh;
  }

  friend
  type operator*(type lh, const T& rh)
  {
    lh *= rh;
    return lh;
  }

  friend
  type operator*(const T& lh, type rh)
  {
    rh *= lh;
    return rh;
  }

  friend
  type operator/(type lh, const T& rh)
  {
    lh /= rh;
    return lh;
  }

  friend
  T operator/(const type& lh, const type& rh)
  {
    return value_of(lh) / value_of(rh);
  }
};

template <typename D>
struct affine_point
{
  template <typename T>
  class modifier;
};

namespace impl
{
  template <typename ...>
  using void_t = void;

  template <typename T, typename = void>
  struct subtractable : std::false_type {};

  template <typename T>
  struct subtractable<T, void_t<decltype(std::declval<const T&>() - std::declval<const T&>())>>
  : std::true_type {};
}


template <typename D>
template <typename T, typename Tag, typename ... M>
class affine_point<D>::modifier<::strong::type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
  static_assert(impl::subtractable<T>::value, "it must be possible to subtract instances of your underlying type");
  using diff_type = decltype(std::declval<const T&>() - std::declval<const T&>());
  static_assert(std::is_constructible<D, diff_type>::value,"");
public:
  STRONG_NODISCARD
  friend
  constexpr
  D
  operator-(
    const type& lh,
    const type& rh)
  {
    return D(value_of(lh) - value_of(rh));
  }

  friend
  type&
  operator+=(
    type& lh,
    const D& d)
  noexcept(noexcept(value_of(lh) += impl::access(d)))
  {
    value_of(lh) += impl::access(d);
    return lh;
  }

  friend
  type&
  operator-=(
    type& lh,
    const D& d)
  noexcept(noexcept(value_of(lh) -= impl::access(d)))
  {
    value_of(lh) -= impl::access(d);
    return lh;
  }

  STRONG_NODISCARD
  friend
  type
  operator+(
    type lh,
    const D& d)
  {
    return lh += d;
  }

  STRONG_NODISCARD
  friend
  type
  operator+(
    const D& d,
    type rh)
  {
    return rh+= d;
  }

  STRONG_NODISCARD
  friend
  type
  operator-(
    type lh,
    const D& d)
  {
    return lh -= d;
  }
};

struct pointer
{
  template <typename T>
  class modifier;
};

template <typename T, typename Tag, typename ... M>
class pointer::modifier<::strong::type<T, Tag, M...>>
{
  using type = strong::type<T, Tag, M...>;
public:
  template <typename TT = T>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator==(
    const type& t,
    std::nullptr_t)
  noexcept(noexcept(std::declval<const TT&>() == nullptr))
  -> decltype(std::declval<const TT&>() == nullptr)
  {
    return value_of(t) == nullptr;
  }

  template <typename TT = T>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator==(
    std::nullptr_t,
    const type& t)
  noexcept(noexcept(nullptr == std::declval<const TT&>()))
  -> decltype(nullptr == std::declval<const TT&>())
  {
    return value_of(t) == nullptr;
  }

  template <typename TT = T>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator!=(
    const type& t,
    std::nullptr_t)
  noexcept(noexcept(std::declval<const TT&>() != nullptr))
  -> decltype(std::declval<const TT&>() != nullptr)
  {
    return value_of(t) != nullptr;
  }

  template <typename TT = T>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator!=(
    std::nullptr_t,
    const type& t)
  noexcept(noexcept(nullptr != std::declval<const TT&>()))
  -> decltype(nullptr != std::declval<const TT&>())
  {
    return value_of(t) != nullptr;
  }

  STRONG_NODISCARD
  constexpr
  decltype(auto)
  operator*()
  const
  {
    auto& self = static_cast<const type&>(*this);
    return *value_of(self);
  }

  constexpr decltype(auto) operator->() const { return &operator*();}
};

struct arithmetic
{
  template <typename T>
  class modifier
  {
  public:
    STRONG_NODISCARD
    friend
    constexpr
    T
    operator-(
      const T &lh)
    {
      return T{-value_of(lh)};
    }

    friend
    constexpr
    T&
    operator+=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) += value_of(rh)))
    {
      value_of(lh) += value_of(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator-=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) -= value_of(rh)))
    {
      value_of(lh) -= value_of(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator*=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) *= value_of(rh)))
    {
      value_of(lh) *= value_of(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator/=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) /= value_of(rh)))
    {
      value_of(lh) /= value_of(rh);
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator+(
      T lh,
      const T &rh)
    {
      lh += rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator-(
      T lh,
      const T &rh)
    {
      lh -= rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator*(
      T lh,
      const T &rh)
    {
      lh *= rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator/(
      T lh,
      const T &rh)
    {
      lh /= rh;
      return lh;
    }
  };
};


struct bitarithmetic
{
  template <typename T>
  class modifier
  {
  public:
    friend
    constexpr
    T&
    operator&=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) &= value_of(rh)))
    {
      value_of(lh) &= value_of(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator|=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) |= value_of(rh)))
    {
      value_of(lh) |= value_of(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator^=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value_of(lh) ^= value_of(rh)))
    {
      value_of(lh) ^= value_of(rh);
      return lh;
    }

    template <typename C>
    friend
    constexpr
    T&
    operator<<=(
      T &lh,
      C c)
    noexcept(noexcept(value_of(lh) <<= c))
    {
      value_of(lh) <<= c;
      return lh;
    }

    template <typename C>
    friend
    constexpr
    T&
    operator>>=(
      T &lh,
      C c)
    noexcept(noexcept(value_of(lh) >>= c))
    {
      value_of(lh) >>= c;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator~(
      const T &lh)
    {
      auto v = value_of(lh);
      v = ~v;
      return T(v);
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator&(
      T lh,
      const T &rh)
    {
      lh &= rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator|(
      T lh,
      const T &rh)
    {
      lh |= rh;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator^(
      T lh,
      const T &rh)
    {
      lh ^= rh;
      return lh;
    }

    template <typename C>
    STRONG_NODISCARD
    friend
    constexpr
    T
    operator<<(
      T lh,
      C c)
    {
      lh <<= c;
      return lh;
    }

    template <typename C>
    STRONG_NODISCARD
    friend
    constexpr
    T
    operator>>(
      T lh,
      C c)
    {
      lh >>= c;
      return lh;
    }
  };
};
template <typename I = void>
struct indexed
{
  template <typename T>
  class modifier;
};

template <>
struct indexed<void> {
  template<typename>
  class modifier;

  template <typename T, typename Tag, typename ... Ms>
  class modifier<type<T, Tag, Ms...>> {
    using ref = T&;
    using cref = const T&;
    using rref = T&&;
    using type = strong::type<T, Tag, Ms...>;
  public:
    template<typename I>
    STRONG_NODISCARD
    auto
    operator[](
      const I &i)
    const &
    noexcept(noexcept(std::declval<cref>()[impl::access(i)]))
    -> decltype(std::declval<cref>()[impl::access(i)]) {
      auto& self = static_cast<const type&>(*this);
      return value_of(self)[impl::access(i)];
    }

    template<typename I>
    STRONG_NODISCARD
    auto
    operator[](
      const I &i)
    &
    noexcept(noexcept(std::declval<ref>()[impl::access(i)]))
    -> decltype(std::declval<ref>()[impl::access(i)]) {
      auto& self = static_cast<type&>(*this);
      return value_of(self)[impl::access(i)];
    }

    template<typename I>
    STRONG_NODISCARD
    auto
    operator[](
      const I &i)
    &&
    noexcept(noexcept(std::declval<rref>()[impl::access(i)]))
    -> decltype(std::declval<rref>()[impl::access(i)]) {
      auto& self = static_cast<type&>(*this);
      return value_of(std::move(self))[impl::access(i)];
    }

    template<typename I, typename C = cref>
    STRONG_NODISCARD
    auto
    at(
      const I &i)
    const &
    -> decltype(std::declval<C>().at(impl::access(i))) {
      auto& self = static_cast<const type&>(*this);
      return value_of(self).at(impl::access(i));
    }

    template<typename I, typename R = ref>
    STRONG_NODISCARD
    auto
    at(
      const I &i)
    &
    -> decltype(std::declval<R>().at(impl::access(i))) {
      auto& self = static_cast<type&>(*this);
      return value_of(self).at(impl::access(i));
    }

    template<typename I, typename R = rref>
    STRONG_NODISCARD
    auto
    at(
      const I &i)
    &&
    -> decltype(std::declval<R>().at(impl::access(i))) {
      auto& self = static_cast<type&>(*this);
      return value_of(std::move(self)).at(impl::access(i));
    }
  };
};

template <typename I>
template <typename T, typename Tag, typename ... M>
class indexed<I>::modifier<type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
public:
  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  const &
  noexcept(noexcept(std::declval<const T&>()[impl::access(i)]))
  -> decltype(std::declval<const T&>()[impl::access(i)])
  {
    auto& self = static_cast<const type&>(*this);
    return value_of(self)[impl::access(i)];
  }

  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  &
  noexcept(noexcept(std::declval<T&>()[impl::access(i)]))
  -> decltype(std::declval<T&>()[impl::access(i)])
  {
    auto& self = static_cast<const type&>(*this);
    return value_of(self)[impl::access(i)];
  }

  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  &&
  noexcept(noexcept(std::declval<T&&>()[impl::access(i)]))
  -> decltype(std::declval<T&&>()[impl::access(i)])
  {
    auto& self = static_cast<type&>(*this);
    return value_of(std::move(self))[impl::access(i)];
  }

  STRONG_NODISCARD
  auto
  at(
    const I& i)
  const &
  -> decltype(std::declval<const T&>().at(impl::access(i)))
  {
    auto& self = static_cast<const type&>(*this);
    return value_of(self).at(impl::access(i));
  }

  STRONG_NODISCARD
  auto
  at(
    const I& i)
  &
  -> decltype(std::declval<T&>().at(impl::access(i)))
  {
    auto& self = static_cast<type&>(*this);
    return value_of(self).at(impl::access(i));
  }

  STRONG_NODISCARD
  auto
  at(
    const I& i)
  &&
  -> decltype(std::declval<T&&>().at(impl::access(i)))
  {
    auto& self = static_cast<type&>(*this);
    return value_of(std::move(self)).at(impl::access(i));
  }
};

class iterator
{
public:
  template <typename I, typename category = typename std::iterator_traits<underlying_type_t<I>>::iterator_category>
  class modifier
    : public pointer::modifier<I>
    , public equality::modifier<I>
    , public incrementable::modifier<I>
  {
    using T = underlying_type_t<I>;
  public:
    using difference_type = typename std::iterator_traits<T>::difference_type;
    using value_type = typename std::iterator_traits<T>::value_type;
    using pointer = typename std::iterator_traits<T>::value_type;
    using reference = typename std::iterator_traits<T>::reference;
    using iterator_category = typename std::iterator_traits<T>::iterator_category;
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

class range
{
public:
  template <typename R>
  class modifier;
};

template <typename T, typename Tag, typename ... M>
class range::modifier<type<T, Tag, M...>>
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
};

namespace impl {

  template<typename T, typename D>
  struct converter
  {
    constexpr explicit operator D() const
    noexcept(noexcept(static_cast<D>(std::declval<const underlying_type_t<T>&>())))
    {
      auto& self = static_cast<const T&>(*this);
      return static_cast<D>(value_of(self));
    }
  };
  template<typename T, typename D>
  struct implicit_converter
  {
    constexpr operator D() const
    noexcept(noexcept(static_cast<D>(std::declval<const underlying_type_t<T>&>())))
    {
      auto& self = static_cast<const T&>(*this);
      return static_cast<D>(value_of(self));
    }
  };
}
template <typename ... Ts>
struct convertible_to
{
  template <typename T>
  struct modifier : impl::converter<T, Ts>...
  {
  };
};

template <typename ... Ts>
struct implicitly_convertible_to
{
  template <typename T>
  struct modifier : impl::implicit_converter<T, Ts>...
  {
  };

};

}

namespace std {
template <typename T, typename Tag, typename ... M>
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
    const ::strong::hashable::modifier<type>& t)
  const
  {
    auto& tt = static_cast<const type&>(t);
    return hash<T>::operator()(value_of(tt));
  }
};
template <typename T, typename Tag, typename ... M>
struct is_arithmetic<::strong::type<T, Tag, M...>>
  : is_base_of<::strong::arithmetic::modifier<::strong::type<T, Tag, M...>>,
               ::strong::type<T, Tag, M...>>
{
};

}
#endif //SAFE_TYPE_SAFE_TYPE_HPP
