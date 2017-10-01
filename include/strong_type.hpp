#ifndef SAFE_TYPE_SAFE_TYPE_HPP
#define SAFE_TYPE_SAFE_TYPE_HPP

#include <utility>
#include <type_traits>
#include <functional>

#if __cplusplus >= 201703L
#define STRONG_NODISCARD [[nodiscard]]
#else
#define STRONG_NODISCARD
#endif

namespace strong
{
template <typename T, typename Tag, typename ... M>
class type : public M::template modifier<type<T, Tag, M...>>...
{
public:
  template <typename ... U,
            typename = std::enable_if_t<std::is_constructible<T, U&&...>{}>>
  constexpr
  explicit
  type(
    U&& ... u)
  noexcept(std::is_nothrow_constructible<T, U...>{})
  : val(std::forward<U>(u)...)
  {}

  template <typename U = T,
            typename = decltype(std::declval<const U&>() == std::declval<const U&>())>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator==(
    const type& lh,
    const type& rh)
  noexcept(noexcept(lh.val == rh.val))
  -> decltype(std::declval<const T&>() == std::declval<const T&>())
  {
    return lh.val == rh.val;
  }

  template <typename U = T,
            typename = decltype(std::declval<const U&>() != std::declval<const U&>())>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator!=(
    const type& lh,
    const type& rh)
  noexcept(noexcept(lh.val != rh.val))
  -> decltype(std::declval<const T&>() != std::declval<const T&>())
  {
    return lh.val != rh.val;
  }

  STRONG_NODISCARD
  constexpr T& value() & noexcept { return val;}
  STRONG_NODISCARD
  constexpr const T& value() const & noexcept { return val;}
  STRONG_NODISCARD
  constexpr T&& value() && noexcept { return std::move(val);}
private:
  T val;
};

template <typename T>
struct is_safe_type : std::false_type {};
template <typename T, typename Tag, typename ... M>
struct is_safe_type<type<T, Tag, M...>> : std::true_type {};

template <typename T, typename = std::enable_if_t<is_safe_type<std::decay_t<T>>{}>>
STRONG_NODISCARD
decltype(auto) value(T&& t) noexcept
{
  return std::forward<T>(t).value();
}

template <typename T>
std::enable_if_t<!is_safe_type<std::decay_t<T>>{}, T&&> value(T&& t) noexcept
{
  return std::forward<T>(t);
}


namespace impl
{
template <typename T, typename U>
static constexpr T &get(U &u) noexcept
{
  static_assert(is_safe_type<T>{}, "");
  static_assert(std::is_base_of<U, T>{}, "");
  return static_cast<T &>(u);
}

template <typename T, typename U>
inline constexpr const T &get(const U &u) noexcept
{
  static_assert(is_safe_type<T>{}, "");
  static_assert(std::is_base_of<U, T>{}, "");
  return static_cast<const T &>(u);
}

template <typename T, typename U>
inline constexpr T &&get(U &&u) noexcept
{
  static_assert(is_safe_type<T>{}, "");
  static_assert(std::is_base_of<std::remove_reference_t<U>, T>{}, "");
  return static_cast<T &&>(static_cast<U&&>(u));
}

template <typename T, typename U>
inline constexpr decltype(auto) access(U &&t) noexcept
{
  static_assert(is_safe_type<T>{}, "");
  static_assert(std::is_base_of<std::remove_reference_t<U>, T>{}, "");
  return get<T>(std::forward<U>(t)).value();
}

template <typename T>
struct type_of;

template <typename T, typename Tag, typename ... M>
struct type_of<::strong::type<T, Tag, M...>>
{
  using type = T;
};

template <typename T, typename Tag, typename ... M>
struct type_of<::strong::type<T, Tag, M...> &>
{
  using type = T &;
};

template <typename T, typename Tag, typename ... M>
struct type_of<::strong::type<T, Tag, M...> &&>
{
  using type = T &&;
};

template <typename T, typename Tag, typename ... M>
struct type_of<const ::strong::type<T, Tag, M...> &>
{
  using type = const T &;
};

template <typename T>
typename type_of<T>::type type_of_t();
}

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
    const modifier& lh,
    const modifier& rh)
  noexcept(noexcept(std::declval<const T&>() < std::declval<const T&>()))
  -> decltype(std::declval<const T&>() < std::declval<const T&>())
  {
    return impl::access<type>(lh) < impl::access<type>(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator<=(
    const modifier& lh,
    const modifier& rh)
  noexcept(noexcept(std::declval<const T&>() <= std::declval<const T&>()))
  -> decltype(std::declval<const T&>() <= std::declval<const T&>())
  {
    return impl::access<type>(lh) <= impl::access<type>(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator>(
    const modifier& lh,
    const modifier& rh)
  noexcept(noexcept(std::declval<const T&>() > std::declval<const T&>()))
  -> decltype(std::declval<const T&>() > std::declval<const T&>())
  {
    return impl::access<type>(lh) > impl::access<type>(rh);
  }

  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator>=(
    const modifier& lh,
    const modifier& rh)
  noexcept(noexcept(std::declval<const T&>() >= std::declval<const T&>()))
  -> decltype(std::declval<const T&>() >= std::declval<const T&>())
  {
    return impl::access<type>(lh) >= impl::access<type>(rh);
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
      const modifier &t)
    {
      return os << impl::access<T>(t);
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
      modifier &t)
    {
      return is >> impl::access<T>(t);
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

struct up_counter
{
  template <typename T>
  class modifier
  {
  public:
    constexpr
    T&
    operator++()
    noexcept(noexcept(++std::declval<T&>().value()))
    {
      auto &self = impl::get<T>(*this);
      ++value(self);
      return self;
    }

    constexpr
    T
    operator++(int)
    {
      T rv{impl::get<T>(*this)};
      ++*this;
      return rv;
    }
  };
};

struct down_counter
{
  template <typename T>
  class modifier
  {
  public:
    constexpr
    T&
    operator--()
    noexcept(noexcept(--std::declval<T&>().value()))
    {
      auto &self = impl::get<T>(*this);
      --value(self);
      return self;
    }

    constexpr
    T
    operator--(int)
    {
      T rv{impl::get<T>(*this)};
      --*this;
      return rv;
    }
  };
};

struct bidirectional_counter
{
  template <typename T>
  class modifier
    : public up_counter::modifier<T>
    , public down_counter::modifier<T>
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
    noexcept(noexcept(static_cast<bool>(impl::type_of_t<T>())))
    {
      return static_cast<bool>(impl::access<T>(*this));
    }
  };
};

struct hashable
{
  template <typename T>
  class modifier{};
};

template <typename D = void>
struct difference
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

template <>
template <typename type>
class difference<void>::modifier
{
  using ref = typename impl::type_of<type&>::type;
  using cref = typename impl::type_of<const type&>::type;
  using rref = typename impl::type_of<type&&>::type;
  static_assert(impl::subtractable<cref>{}, "it must be possible to subtract instances of your underlying type");
public:
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator-(
    const modifier& lh,
    const modifier& rh)
  -> decltype(std::declval<cref>() - std::declval<cref>())
  {
    return impl::access<type>(lh) - impl::access<type>(rh);
  }

  template <typename D>
  type&
  operator+=(
    const D& d)
  noexcept(noexcept(std::declval<ref>() += value(d)))
  {
    impl::access<type>(*this) += value(d);
    return impl::get<type>(*this);
  }
  template <typename D>
  type&
  operator-=(
    const D& d)
  noexcept(noexcept(std::declval<ref>() -= value(d)))
  {
    impl::access<type>(*this) -= value(d);
    return impl::get<type>(*this);
  }

  template <typename D>
  STRONG_NODISCARD
  friend
  type
  operator+(
    const modifier& lh,
    const D& d)
  {
    return type(impl::access<type>(lh) + value(d));
  }

  template <typename D>
  STRONG_NODISCARD
  friend
  type
  operator+(
    const D& d,
    const modifier& rh)
  {
    return type(value(d) + impl::access<type>(rh));
  }

  template <typename D>
  STRONG_NODISCARD
  friend
  type
  operator-(
    const modifier& lh,
    const D& d)
  {
    return type(impl::access<type>(lh) - value(d));
  }
};

template <typename D>
template <typename T, typename Tag, typename ... M>
class difference<D>::modifier<::strong::type<T, Tag, M...>>
{
  using type = ::strong::type<T, Tag, M...>;
  static_assert(impl::subtractable<T>{}, "it must be possible to subtract instances of your underlying type");
  using diff_type = decltype(std::declval<const T&>() - std::declval<const T&>());
  static_assert(std::is_constructible<D, diff_type>{},"");
public:
  STRONG_NODISCARD
  friend
  constexpr
  D
  operator-(
    const modifier& lh,
    const modifier& rh)
  {
    return D(impl::access<type>(lh) - impl::access<type>(rh));
  }

  type&
  operator+=(
    const D& d)
  noexcept(noexcept(std::declval<T&>() += value(d)))
  {
    impl::access<type>(*this) += value(d);
    return impl::get<type>(*this);
  }

  type&
  operator-=(
    const D& d)
  noexcept(noexcept(std::declval<T&>() -= value(d)))
  {
    impl::access<type>(*this) -= value(d);
    return impl::get<type>(*this);
  }

  STRONG_NODISCARD
  friend
  type
  operator+(
    const modifier& lh,
    const D& d)
  {
    return type(impl::access<type>(lh) + value(d));
  }

  STRONG_NODISCARD
  friend
  type
  operator+(
    const D& d,
    const modifier& rh)
  {
    return type(value(d) + impl::access<type>(rh));
  }

  STRONG_NODISCARD
  friend
  type
  operator-(
    const modifier& lh,
    const D& d)
  {
    return type(impl::access<type>(lh) - value(d));
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
  template <typename U = type>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator==(
    const type& t,
    std::nullptr_t)
  noexcept(noexcept(std::declval<const T&>() == nullptr))
  -> decltype(std::declval<const T&>() == nullptr)
  {
    return value(t) == nullptr;
  }

  template <typename U = type>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator==(
    std::nullptr_t,
    const type& t)
  noexcept(noexcept(nullptr == std::declval<const T&>()))
  -> decltype(nullptr == std::declval<const T&>())
  {
    return value(t) == nullptr;
  }

  template <typename U = type>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator!=(
    const type& t,
    std::nullptr_t)
  noexcept(noexcept(std::declval<const T&>() != nullptr))
  -> decltype(std::declval<const T&>() != nullptr)
  {
    return value(t) != nullptr;
  }

  template <typename U = type>
  STRONG_NODISCARD
  friend
  constexpr
  auto
  operator!=(
    std::nullptr_t,
    const type& t)
  noexcept(noexcept(nullptr != std::declval<const T&>()))
  -> decltype(nullptr != std::declval<const T&>())
  {
    return value(t) != nullptr;
  }

  STRONG_NODISCARD
  constexpr
  decltype(auto)
  operator*()
  const
  {
    return *impl::access<type>(*this);
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
      return T{-value(lh)};
    }

    friend
    constexpr
    T&
    operator+=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value(lh) += value(rh)))
    {
      value(lh) += value(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator-=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value(lh) -= value(rh)))
    {
      value(lh) -= value(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator*=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value(lh) *= value(rh)))
    {
      value(lh) *= value(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator/=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value(lh) /= value(rh)))
    {
      value(lh) /= value(rh);
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
    noexcept(noexcept(value(lh) &= value(rh)))
    {
      value(lh) &= value(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator|=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value(lh) |= value(rh)))
    {
      value(lh) |= value(rh);
      return lh;
    }

    friend
    constexpr
    T&
    operator^=(
      T &lh,
      const T &rh)
    noexcept(noexcept(value(lh) ^= value(rh)))
    {
      value(lh) ^= value(rh);
      return lh;
    }

    template <typename C>
    friend
    constexpr
    T&
    operator<<=(
      T &lh,
      C c)
    noexcept(noexcept(value(lh) <<= c))
    {
      value(lh) <<= c;
      return lh;
    }

    template <typename C>
    friend
    constexpr
    T&
    operator>>=(
      T &lh,
      C c)
    noexcept(noexcept(value(lh) >>= c))
    {
      value(lh) >>= c;
      return lh;
    }

    STRONG_NODISCARD
    friend
    constexpr
    T
    operator~(
      const T &lh)
    {
      auto v = value(lh);
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
template <typename Type>
class indexed<void>::modifier
{
  using ref = typename impl::type_of<Type&>::type;
  using cref = typename impl::type_of<const Type&>::type;
  using rref = typename impl::type_of<Type&&>::type;
public:
  template <typename I>
  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  const &
  noexcept(noexcept(std::declval<cref>()[strong::value(i)]))
  -> decltype(std::declval<cref>()[strong::value(i)])
  {
    return impl::access<Type>(*this)[strong::value(i)];
  }

  template <typename I>
  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  &
  noexcept(noexcept(std::declval<ref>()[strong::value(i)]))
  -> decltype(std::declval<ref>()[strong::value(i)])
  {
    return impl::access<Type>(*this)[strong::value(i)];
  }

  template <typename I>
  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  &&
  noexcept(noexcept(std::declval<rref>()[strong::value(i)]))
  -> decltype(std::declval<rref>()[strong::value(i)])
  {
    return impl::access<Type>(*this)[strong::value(i)];
  }
  template <typename I>
  STRONG_NODISCARD
  auto
  at(
    const I& i)
  const &
  -> decltype(std::declval<cref>().at(strong::value(i)))
  {
    return impl::access<Type>(*this).at(strong::value(i));
  }
  template <typename I>
  STRONG_NODISCARD
  auto
  at(
    const I& i)
  &
  -> decltype(std::declval<ref>().at(strong::value(i)))
  {
    return impl::access<Type>(*this).at(strong::value(i));
  }
  template <typename I>
  STRONG_NODISCARD
  auto
  at(
    const I& i)
  &&
  -> decltype(std::declval<rref>().at(strong::value(i)))
  {
    return impl::access<Type>(*this).at(strong::value(i));
  }
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
  noexcept(noexcept(std::declval<const T&>()[strong::value(i)]))
  -> decltype(std::declval<const T&>()[strong::value(i)])
  {
    return impl::access<type>(*this)[strong::value(i)];
  }

  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  &
  noexcept(noexcept(std::declval<T&>()[strong::value(i)]))
  -> decltype(std::declval<T&>()[strong::value(i)])
  {
    return impl::access<type>(*this)[strong::value(i)];
  }

  STRONG_NODISCARD
  auto
  operator[](
    const I& i)
  &&
  noexcept(noexcept(std::declval<T&&>()[strong::value(i)]))
  -> decltype(std::declval<T&&>()[strong::value(i)])
  {
    return impl::access<type>(*this)[strong::value(i)];
  }

  STRONG_NODISCARD
  auto
  at(
    const I& i)
  const &
  -> decltype(std::declval<const T&>().at(strong::value(i)))
  {
    return impl::access<type>(*this).at(strong::value(i));
  }

  STRONG_NODISCARD
  auto
  at(
    const I& i)
  &
  -> decltype(std::declval<T&>().at(strong::value(i)))
  {
    return impl::access<type>(*this).at(strong::value(i));
  }

  STRONG_NODISCARD
  auto
  at(
    const I& i)
  &&
  -> decltype(std::declval<T&&>().at(strong::value(i)))
  {
    return impl::access<type>(*this).at(strong::value(i));
  }
};

}

namespace std {
template <typename T, typename Tag, typename ... M>
struct hash<::strong::type<T, Tag, M...>> : hash<T>
{
  using type = ::strong::type<T, Tag, M...>;
  decltype(auto)
  operator()(
    const ::strong::hashable::modifier<type>& t)
  const
  {
    return hash<T>::operator()(::strong::impl::access<type>(t));
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
