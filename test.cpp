#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <strong_type.hpp>
#include <unordered_set>

template <typename T, typename U>
using equality_compare = decltype(std::declval<const T&>() == std::declval<const U&>());

template <typename T, typename U>
using indexing = decltype(std::declval<const T&>()[std::declval<const U&>()]);

template <typename T>
using hash_type = decltype(std::declval<std::hash<T>>()(std::declval<const T&>()));

template <typename T, typename U>
using less_than_compare = decltype(std::declval<const T&>() < std::declval<const U&>());

template <typename T, typename U>
using subtract = decltype(std::declval<const T&>() - std::declval<const U&>());

template <typename T, typename U>
using add = decltype(std::declval<const T&>() + std::declval<const U&>());

template <typename T>
using ostream_insertion = decltype(std::declval<std::ostream&>() << std::declval<const T&>());

template <typename T>
using istream_extraction = decltype(std::declval<std::istream&>() >> std::declval<T&>());

template <typename T>
using preincrement = decltype(++std::declval<T&>());

template <typename T>
using predecrement = decltype(--std::declval<T&>());

template <typename ...>
struct void_t_
{
  using type = void;
};

template <typename ... T>
using void_t = typename void_t_<T...>::type;

template <template <typename...> class, typename...>
struct is_detected_ : std::false_type {};

template <template <typename...> class D, typename ...T>
struct is_detected_<D, void_t<D<T...>>, T...> : std::true_type {};

template <template <typename ...> class D, typename ... T>
using is_detected = typename is_detected_<D, void, T...>::type;

template <typename T, typename U = T>
using is_equal_comparable = is_detected<equality_compare, T, U>;

template <typename T, typename U = T>
using is_less_than_comparable = is_detected<less_than_compare, T, U>;

template <typename T>
using is_ostreamable = is_detected<ostream_insertion, T>;

template <typename T>
using is_istreamable = is_detected<istream_extraction, T>;

template <typename T>
using is_decrementable = is_detected<predecrement, T>;

template <typename T>
using is_incrementable = is_detected<preincrement, T>;

template <typename T>
using is_hashable = is_detected<hash_type, T>;

template <typename T, typename U = T>
using is_subtractable = is_detected<subtract, T, U>;

template <typename T, typename U = T>
using is_addable = is_detected<add, T, U>;

template <typename T, typename U>
using is_indexable = is_detected<indexing, T, U>;

using handle = strong::type<int, struct handle_tag>;

static_assert(std::is_lvalue_reference<decltype(value(std::declval<handle&>()))>{},"");
static_assert(std::is_lvalue_reference<decltype(value(std::declval<const handle&>()))>{},"");
static_assert(std::is_const<std::remove_reference_t<decltype(value(std::declval<const handle&>()))>>{},"");
static_assert(std::is_rvalue_reference<decltype(value(std::declval<handle&&>()))>{},"");

static_assert(std::is_nothrow_default_constructible<handle>{},"");
static_assert(std::is_nothrow_constructible<handle, int&&>{},"");
static_assert(std::is_nothrow_constructible<handle, const int&>{},"");
static_assert(std::is_copy_constructible<handle>{},"");
static_assert(is_equal_comparable<handle>{}, "");
static_assert(std::is_nothrow_assignable<handle, const handle&>{}, "");
static_assert(std::is_nothrow_assignable<handle, handle&&>{}, "");
static_assert(!is_less_than_comparable<handle>{},"");
static_assert(!is_ostreamable<handle>{},"");
static_assert(!is_istreamable<handle>{},"");
static_assert(!std::is_constructible<bool, handle>{}, "");
static_assert(!is_incrementable<handle>{},"");
static_assert(!is_decrementable<handle>{},"");
static_assert(!std::is_arithmetic<handle>{},"");
static_assert(!is_hashable<handle>{},"");
static_assert(!is_indexable<handle, int>{}, "");

using handle2 = strong::type<int, struct handle2_tag>;

static_assert(!std::is_constructible<handle, const handle2&>{},"");
static_assert(!std::is_constructible<handle, handle2&&>{},"");
static_assert(!std::is_assignable<handle, const handle2&>{},"");
static_assert(!std::is_assignable<handle, handle2&&>{},"");
static_assert(!is_equal_comparable<handle, handle2>{},"");

using bhandle = strong::type<int, struct bhandle_tag, strong::boolean>;

static_assert(std::is_nothrow_default_constructible<bhandle>{},"");
static_assert(std::is_nothrow_constructible<bhandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<bhandle, const int&>{},"");
static_assert(std::is_copy_constructible<bhandle>{},"");
static_assert(is_equal_comparable<bhandle>{}, "");
static_assert(std::is_nothrow_assignable<bhandle, const bhandle&>{}, "");
static_assert(std::is_nothrow_assignable<bhandle, bhandle&&>{}, "");
static_assert(!is_less_than_comparable<bhandle>{},"");
static_assert(!is_ostreamable<bhandle>{},"");
static_assert(!is_istreamable<bhandle>{},"");
static_assert(std::is_constructible<bool, bhandle>{}, "");
static_assert(!is_incrementable<bhandle>{},"");
static_assert(!is_decrementable<bhandle>{},"");
static_assert(!std::is_arithmetic<bhandle>{},"");
static_assert(!is_hashable<bhandle>{},"");
static_assert(!is_indexable<bhandle, int>{}, "");

using ahandle = strong::type<int, struct ahandle_tag, strong::arithmetic>;

static_assert(std::is_nothrow_default_constructible<ahandle>{},"");
static_assert(std::is_nothrow_constructible<ahandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<ahandle, const int&>{},"");
static_assert(std::is_copy_constructible<ahandle>{},"");
static_assert(is_equal_comparable<ahandle>{}, "");
static_assert(std::is_nothrow_assignable<ahandle, const ahandle&>{}, "");
static_assert(std::is_nothrow_assignable<ahandle, ahandle&&>{}, "");
static_assert(!is_less_than_comparable<ahandle>{},"");
static_assert(!is_ostreamable<ahandle>{},"");
static_assert(!is_istreamable<ahandle>{},"");
static_assert(!std::is_constructible<bool, ahandle>{}, "");
static_assert(!is_incrementable<ahandle>{},"");
static_assert(!is_decrementable<ahandle>{},"");
static_assert(std::is_arithmetic<ahandle>{},"");
static_assert(!is_hashable<ahandle>{},"");
static_assert(is_subtractable<ahandle>{}, "");
static_assert(!is_subtractable<ahandle, bhandle>{},"");
static_assert(!is_indexable<ahandle, int>{}, "");

using ahandle2 = strong::type<int, struct ahandle2_tag, strong::arithmetic>;

static_assert(!is_subtractable<ahandle, ahandle2>{},"");

using hhandle = strong::type<int, struct hhandle_tag, strong::hashable>;

static_assert(std::is_nothrow_default_constructible<hhandle>{},"");
static_assert(std::is_nothrow_constructible<hhandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<hhandle, const int&>{},"");
static_assert(std::is_copy_constructible<hhandle>{},"");
static_assert(is_equal_comparable<hhandle>{}, "");
static_assert(std::is_nothrow_assignable<hhandle, const hhandle&>{}, "");
static_assert(std::is_nothrow_assignable<hhandle, hhandle&&>{}, "");
static_assert(!is_less_than_comparable<hhandle>{},"");
static_assert(!is_ostreamable<hhandle>{},"");
static_assert(!is_istreamable<hhandle>{},"");
static_assert(!std::is_constructible<bool, hhandle>{}, "");
static_assert(!is_incrementable<hhandle>{},"");
static_assert(!is_decrementable<hhandle>{},"");
static_assert(!std::is_arithmetic<hhandle>{},"");
static_assert(is_hashable<hhandle>{},"");
static_assert(!is_indexable<hhandle, int>{}, "");

using ihandle = strong::type<std::string, struct string_tag, strong::indexed<int>>;

static_assert(std::is_nothrow_default_constructible<ihandle>{},"");
static_assert(!std::is_nothrow_constructible<ihandle, int>{},"");
static_assert(std::is_copy_constructible<ihandle>{},"");
static_assert(is_equal_comparable<ihandle>{}, "");
static_assert(!std::is_nothrow_assignable<ihandle, const ihandle&>{}, "");
static_assert(std::is_nothrow_assignable<ihandle, ihandle&&>{}, "");
static_assert(!is_less_than_comparable<ihandle>{},"");
static_assert(!is_ostreamable<ihandle>{},"");
static_assert(!is_istreamable<ihandle>{},"");
static_assert(!std::is_constructible<bool, ihandle>{}, "");
static_assert(!is_incrementable<ihandle>{},"");
static_assert(!is_decrementable<ihandle>{},"");
static_assert(!std::is_arithmetic<ihandle>{},"");
static_assert(!is_hashable<ihandle>{},"");
static_assert(is_indexable<ihandle, int>{}, "");

using dhandle = strong::type<int, struct int_tag, strong::difference<handle>>;

static_assert(std::is_nothrow_default_constructible<dhandle>{},"");
static_assert(std::is_nothrow_constructible<dhandle, int>{},"");
static_assert(std::is_copy_constructible<dhandle>{},"");
static_assert(is_equal_comparable<dhandle>{}, "");
static_assert(std::is_nothrow_assignable<dhandle, const dhandle&>{}, "");
static_assert(std::is_nothrow_assignable<dhandle, dhandle&&>{}, "");
static_assert(!is_less_than_comparable<dhandle>{},"");
static_assert(!is_ostreamable<dhandle>{},"");
static_assert(!is_istreamable<dhandle>{},"");
static_assert(!std::is_constructible<bool, dhandle>{}, "");
static_assert(!is_incrementable<dhandle>{},"");
static_assert(!is_decrementable<dhandle>{},"");
static_assert(!std::is_arithmetic<dhandle>{},"");
static_assert(!is_hashable<dhandle>{},"");
static_assert(!is_indexable<dhandle, int>{}, "");
static_assert(is_subtractable<dhandle,handle>{}, "");
static_assert(is_subtractable<dhandle,dhandle>{},"");
static_assert(!is_addable<dhandle,dhandle>{},"");
static_assert(is_addable<dhandle,handle>{},"");
static_assert(is_addable<handle,dhandle>{},"");

TEST_CASE("Construction from a value type lvalue copies it")
{
  auto orig = std::make_shared<int>(3);
  strong::type<std::shared_ptr<int>, struct int_> i{orig};
  REQUIRE(*value(i) == 3);
  REQUIRE(orig.use_count() == 2);
  REQUIRE(value(i).get() == orig.get());
}

TEST_CASE("Construction from multiple parameters constructs value from them")
{
  strong::type<std::string, struct string_> s(3U, 'a');
  REQUIRE(value(s) == "aaa");
}

TEST_CASE("construction from an rvalue reference moves from it")
{
  auto orig = std::make_unique<int>(3);
  auto addr = orig.get();
  strong::type<std::unique_ptr<int>, struct p_, strong::pointer> p(std::move(orig));
  REQUIRE(*p == 3);
  REQUIRE(!orig);
  REQUIRE(value(p).get() == addr);
}

TEST_CASE("value can be copy constructed if member can")
{
  using ptr = strong::type<std::shared_ptr<int>, struct p_, strong::pointer>;
  ptr orig{std::make_shared<int>(3)};
  ptr p{orig};
  REQUIRE(value(orig).use_count() == 2);
  REQUIRE(*p == 3);
}

TEST_CASE("value can be copy assigned")
{
  using ptr = strong::type<std::shared_ptr<int>, struct p_, strong::pointer>;
  ptr orig{std::make_shared<int>(3)};
  ptr p;
  p = orig;
  REQUIRE(value(orig).use_count() == 2);
  REQUIRE(*p == 3);
}

TEST_CASE("value can be move assigned")
{
  using ptr = strong::type<std::unique_ptr<int>, struct p_, strong::pointer>;
  ptr orig{std::make_unique<int>(3)};
  ptr p;
  p = std::move(orig);
  REQUIRE(orig == nullptr);
  REQUIRE(*p == 3);
}

TEST_CASE("value can be retained from const lvalue ref")
{
  const strong::type<int, struct i_> i{3};
  auto&& r = value(i);
  REQUIRE(r == 3);
  static_assert(std::is_const<std::remove_reference_t <decltype(r)>>{},"");
  static_assert(std::is_lvalue_reference<decltype(r)>{},"");
}

TEST_CASE("value can be retained from rvalue ref")
{
  strong::type<int, struct i_> i{3};
  auto&& r = value(std::move(i));
  REQUIRE(r == 3);
  static_assert(!std::is_const<std::remove_reference_t <decltype(r)>>{},"");
  static_assert(std::is_rvalue_reference<decltype(r)>{},"");
}

TEST_CASE("values can be compared using operator==")
{
  using type = strong::type<int, struct i_>;

  type i{3};
  type j{4};
  type k{3};
  REQUIRE_FALSE(i == j);
  REQUIRE(i == k);
}

TEST_CASE("values can be compared using operator!=")
{
  using type = strong::type<int, struct i_>;

  type i{3};
  type j{4};
  type k{3};
  REQUIRE(i != j);
  REQUIRE(!(i != k));
}

TEST_CASE("ordered type can be compared for ordering")
{
  using type = strong::type<int, struct i_, strong::ordered>;

  const type i1{1};
  const type i2{2};

  REQUIRE(i1 < i2);
  REQUIRE(i1 <= i2);
  REQUIRE(i1 <= i1);
  REQUIRE(i2 > i1);
  REQUIRE(i2 >= i2);
  REQUIRE(i2 >= i1);

  REQUIRE_FALSE(i2 < i1);
  REQUIRE_FALSE(i2 <= i1);
  REQUIRE_FALSE(i1 > i2);
  REQUIRE_FALSE(i1 >= i2);
}

TEST_CASE("an ostreamable type can be streamed using stream flags")
{
  strong::type<int, struct i_, strong::ostreamable> i{3};

  std::ostringstream oss;
  oss << std::setw(4) << i;
  REQUIRE(oss.str() == "   3");
}

TEST_CASE("an istreamable type can be read from a istream")
{
  strong::type<int, struct i_, strong::istreamable> i{0}, j{0};

  std::istringstream is{" 3 4"};
  is >> i >> j;
  REQUIRE(value(i) == 3);
  REQUIRE(value(j) == 4);
}

TEST_CASE("an iostreamable type can be both read and written using streams")
{
  strong::type<int, struct i_, strong::iostreamable> i{0};
  std::istringstream is{"3"};
  is >> i;
  std::ostringstream os;
  os << i;
  REQUIRE(os.str() == "3");
}

TEST_CASE("an up_counter can be incremented")
{
  using C = strong::type<int, struct i_, strong::up_counter>;

  C c{3};
  REQUIRE(++c == C{4});
  REQUIRE(c == C{4});
  REQUIRE(c++ == C{4});
  REQUIRE(c == C{5});
}

TEST_CASE("a down_counter can be decremented")
{
  using C = strong::type<int, struct i_, strong::down_counter>;

  C c{10};
  REQUIRE(--c == C{9});
  REQUIRE(c == C{9});
  REQUIRE(c-- == C{9});
  REQUIRE(c == C{8});
}

TEST_CASE("a bidirectional_counter goes both ways")
{
  using C = strong::type<int, struct i_, strong::bidirectional_counter>;

  C c{3};
  REQUIRE(++c == C{4});
  REQUIRE(c == C{4});
  REQUIRE(c++ == C{4});
  REQUIRE(c == C{5});
  REQUIRE(--c == C{4});
  REQUIRE(c == C{4});
  REQUIRE(c-- == C{4});
  REQUIRE(c == C{3});
}

TEST_CASE("a boolean value can be tested for truth")
{
  using I = strong::type<int, struct i_, strong::boolean>;

  I i{3};
  REQUIRE(i);
  REQUIRE_FALSE(!i);
  i = I{0};
  REQUIRE(!i);
  REQUIRE_FALSE(i);
}

TEST_CASE("pointer types can be compared with nullptr")
{
  using P = strong::type<std::unique_ptr<int>, struct p_, strong::pointer>;

  P p;
  REQUIRE(p == nullptr);
  REQUIRE(nullptr == p);
  REQUIRE_FALSE(p != nullptr);
  REQUIRE_FALSE(nullptr != p);

  p = P{std::make_unique<int>(3)};

  REQUIRE_FALSE(p == nullptr);
  REQUIRE_FALSE(nullptr == p);
  REQUIRE(p != nullptr);
  REQUIRE(nullptr != p);
}

TEST_CASE("pointer types can be dereferenced using operator ->")
{
  struct S { int i;};
  S s {3};
  strong::type<S*, struct s_, strong::pointer> p{&s};

  REQUIRE(p->i == 3);
}

TEST_CASE("pointer types can be dereferenced using operator *")
{
  int i = 0;
  strong::type<int*, struct s_, strong::pointer> p{&i};
  *p = 3;
  REQUIRE(i == 3);
}

TEST_CASE("arithmetic types can be added")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{3};
  T j{4};
  REQUIRE(i + j == T{7});
  REQUIRE(i == T{3});
  REQUIRE(j == T{4});
  i += j;
  REQUIRE(i == T{7});
  REQUIRE(j == T{4});
}

TEST_CASE("arithmetic types can be subtracted")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{7};
  T j{4};
  REQUIRE(i - j == T{3});
  REQUIRE(i == T{7});
  REQUIRE(j == T{4});
  i -= j;
  REQUIRE(i == T{3});
  REQUIRE(j == T{4});
}

TEST_CASE("arithmetic types can be multiplied")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{12};
  T j{3};
  REQUIRE(i * j == T{36});
  REQUIRE(i == T{12});
  REQUIRE(j == T{3});
  i *= j;
  REQUIRE(i == T{36});
  REQUIRE(j == T{3});
}

TEST_CASE("arithmetic types can be divided")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{12};
  T j{3};
  REQUIRE(i / j == T{4});
  REQUIRE(i == T{12});
  REQUIRE(j == T{3});
  i /= j;
  REQUIRE(i == T{4});
  REQUIRE(j == T{3});
}

TEST_CASE("an aritmmetic type can be negated")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{12};
  REQUIRE(-i == T{-12});
  REQUIRE(i == T{12});
}

TEST_CASE("bitarithmetic types can be bitanded")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};
  T j{u(0x5555)};

  REQUIRE((i & j) == T{u(0x0505)});
  REQUIRE(i == T{u(0x0f0f)});
  REQUIRE(j == T{u(0x5555)});
  REQUIRE((i &= j) == T{u(0x0505)});
  REQUIRE(i == T{u(0x0505)});
  REQUIRE(j == T{u(0x5555)});
}

TEST_CASE("bitarithmetic types can be bitored")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};
  T j{u(0x5555)};

  REQUIRE((i | j) == T{u(0x5f5f)});
  REQUIRE(i == T{u(0x0f0f)});
  REQUIRE(j == T{u(0x5555)});
  REQUIRE((i |= j) == T{u(0x5f5f)});
  REQUIRE(i == T{u(0x5f5f)});
  REQUIRE(j == T{u(0x5555)});
}

TEST_CASE("bitarithmetic types can be bitxored")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};
  T j{u(0x5555)};

  REQUIRE((i ^ j) == T{u(0x5a5a)});
  REQUIRE(i == T{u(0x0f0f)});
  REQUIRE(j == T{u(0x5555)});
  REQUIRE((i ^= j) == T{u(0x5a5a)});
  REQUIRE(i == T{u(0x5a5a)});
  REQUIRE(j == T{u(0x5555)});
}

TEST_CASE("bitarithmetic types can be left shifted")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};

  REQUIRE((i << 2) == T{u(0x3c3c)});
  REQUIRE(i == T{u(0x0f0f)});
  REQUIRE((i <<= 3) == T{u(0x7878)});
  REQUIRE(i == T{u(0x7878)});
}

TEST_CASE("bitarithmetic types can be right shifted")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};

  REQUIRE((i >> 2) == T{u(0x03c3)});
  REQUIRE(i == T{u(0x0f0f)});
  REQUIRE((i >>= 1) == T{u(0x0787)});
  REQUIRE(i == T{u(0x0787)});
}

TEST_CASE("bitarithmetic types can be inverted")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};

  REQUIRE(~i == T{u(0xf0f0)});
  REQUIRE(i == T{u(0x0f0f)});
}

TEST_CASE("indexed can be accessed using operator []")
{
  using I = strong::type<unsigned, struct I_>;
  using T = strong::type<std::string, struct s_, strong::indexed<>>;

  T s("foo");
  const T c("bar");

  auto& r = s[I{0U}];
  static_assert(!std::is_const<std::remove_reference_t<decltype(r)>>{}, "");
  REQUIRE(r == 'f');
  auto& cr = c[I{0U}];
  static_assert(std::is_const<std::remove_reference_t<decltype(cr)>>{}, "");
  REQUIRE(cr == 'b');

  auto& ar = s.at(I{0U});
  static_assert(!std::is_const<std::remove_reference_t<decltype(ar)>>{}, "");
  REQUIRE(ar == 'f');
  auto& acr = c.at(I{0U});
  static_assert(std::is_const<std::remove_reference_t<decltype(acr)>>{}, "");
  REQUIRE(acr == 'b');
}

TEST_CASE("difference types can be subtracted")
{
  using D = strong::type<int, struct i_>;
  using T = strong::type<int, struct i_, strong::difference<D>>;

  T t1{3};
  T t2{8};
  auto d = t2 - t1;
  static_assert(std::is_same<decltype(d), D>{}, "");
  REQUIRE(d == D{5});
}

TEST_CASE("difference types can be added with the delta type")
{
  using D = strong::type<int, struct i_>;
  using T = strong::type<int, struct i_, strong::difference<D>>;

  T t1{8};
  D d{3};

  auto t2 = t1 + d;
  static_assert(std::is_same<decltype(t2), T>{}, "");
  REQUIRE(value(t2) == 11);
  auto t3 = d + t1;
  static_assert(std::is_same<decltype(t3), T>{}, "");
  REQUIRE(value(t3) == 11);
  t1 += d;
  REQUIRE(t1 == T{11});
}

TEST_CASE("difference types can be subtracted with the delta type")
{
  using D = strong::type<int, struct i_>;
  using T = strong::type<int, struct i_, strong::difference<D>>;

  T t1{8};
  D d{3};

  auto t2 = t1 - d;
  static_assert(std::is_same<decltype(t2), T>{}, "");
  REQUIRE(value(t2) == 5);
  t1 -= d;
  REQUIRE(t1 == T{5});
}