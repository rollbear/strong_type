# strong_type
An additive strong typedef library for C++14/17/20 using the
Boost Software License 1.0

[![CI Build Status](https://github.com/rollbear/strong_type/actions/workflows/ci.yml/badge.svg)](https://github.com/rollbear/strong_type/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/rollbear/strong_type/branch/main/graph/badge.svg?token=cS97dA6jRV)](https://codecov.io/gh/rollbear/strong_type)

* [Intro](#intro)
* [Modifiers](#modifiers)
* [Utilities](#utilities)
* [Self test](#selftest)
* [Other libraries](#other)
* [Presentations](#presentations)

# <A name="intro"/>Intro

Very much inspired by [@foonathan's](https://twitter.com/foonathan)
[`type_safe`](https://github.com/foonathan/type_safe) library, but aim is
slightly different. Limit scope for type safety only. No runtime checks. Also
strive for a higher level abstraction of the needed functionality. The idea
is to suffer no runtime penalty, but to capture misuse at compile time
(for example accidentally subtracting from a handle, or swapping two parameters
in a function call) while still being easy to use for inexperienced
programmers.

Example use:

```Cpp
#include <strong_type/strong_type.hpp>
using myint = strong::type<int, struct my_int_>;
```

`myint` is a very basic handle. You can initialize it. You can do
equal/not-equal comparison with other instances of the same type, and you can
access its underlying `int` instance with `value_of(variable)`.

To get the underlying type of a strong type, use
`typename strong::underlying_type<mytype>::type`, or the convenience alias
`strong::underlying_type_t<mytype>`. If `mytype` is not a `strong::type`,
they give `mytype`.

```Cpp
using otherint = strong::type<int, struct other_int_>;
```

`otherint` is a distinct type from `myint`. If a function takes an argument of
type `myint`, you can't pass it an instance of `otherint`, and vice-versa. You
also can't cross-assign, cross-create or cross-compare.

To access more functionality, you add modifiers. For example:

```Cpp
using ordered_int = strong::type<int, struct ordered_int_, strong::ordered>;
```

Type `ordered_int` now supports relational order comparisons, like `<`,
(provided the underlying type, int this case `int`, does.) Type `ordered_int`
can thus be used as key in `std::map`<> or `std::set<>`.

The header file `<strong_type/strong_type.hpp>` brings you all functionality.
There are more fine-grained headers available, which may speed up builds in
some situations.

# <A name="modifiers"/> Modifiers:

* <A name="affine_point"/>`strong::affine_point<D>` allows instances to be
  subtracted (yielding a `D`) or to add or subtract a `D` to an instance.
  See [Affine Space](https://en.wikipedia.org/wiki/Affine_space). Examples of
  one dimentional affine points are pointer (with `D` being `ptrdiff_t`,) or
  `std::time_point<>` (with `std::duration<>` as `D`.) An example of a
  multidimensional affine point is a coordinate (with a vector type for `D`.)
  `D` can be defaulted, using `strong::affine_point<>`, in which case the
  difference type shares the same tag.
  The difference type from a `strong::affine_point<D>` can be obtained using
  `type::difference`, regardless of whether `D` is explicit or
  defaulted.  It is natural that `D` is of a [`strong::difference`](#difference)
  type. This is a good name from a mathematical point of view, but perhaps a bit
  too academic, and not well aligned with the other names.

  Available in `strong_type//affine_point.hpp`.


* <A name="arithmetic"/>`strong::arithmetic` allows addition, subtraction,
  multiplication, division and remainder of instances.

  Available in `strong_type/arithmetic.hpp`.


* <A name="bicrementable"/>`strong::bicrementable`. Obviously a made up word
  for the occation. Implements both
  [`strong::incrementable`](#incrementable) and
  [`strong::decrementable`](#decrementable).

  Available in `strong_type/bicrementable.hpp`


* <A name="bitarithmetic"/>`strong::bitarithmetic` allows bitwise `&`, bitwise
  `|`, bitwise `^` and shift operations.

  Available in `strong_type/bitarithmetic.hpp`.


* <A name="boolean"/>`strong::boolean` provides
  `explicit operator bool() const`, providing the
  underlying type supports it.

  Available in `strong_type/boolean.hpp`.


* <A name="convertible_to"/>`strong::convertible_to<Ts...>` provides an
  `explicit operator Ts() const` for each type `Ts`, providing the underlying
  type supports it.

  Available in `strong_type/convertible_to.hpp`.


* <A name="decrementable"/>`strong::decrementable`. Provides
  [`operator--](https://en.cppreference.com/w/cpp/language/operator_incdec) for
  the strong type, using the operator of the underlying type.

  Available in `strong_type/incrementable.hpp`


* <A name="default_constructible"/>`strong::default_constructible`. The strong
  type is not default constructible by default. This modifier enables a default
  constructor which uses a default constructor of the underlying type.

  Available in `strong_type/type.hpp`


* <A name="difference"/>`strong::difference` allows instances to be subtracted
  and added (yielding a `strong::difference`).

  Conditionally, if the underlying
  type supports it, `strong_difference` is ordered, may be divided (yielding
  the base type), or multiplied or divided with the base type, yielding another
  `strong::difference`. Also, conditionally, the remainder
  after division of two differences yields the underlying type, and the
  remainder after division of a difference and the underlying type yields a
  difference.  A `strong::difference` is also [`strong::equality`](#equality).

  Available in `strong_type/difference.hpp`.


* <A name="equality"/>`strong::equality` provides operators `==` and `!=`. The
  strong type can be compared for equality or inequality.

  Available in `strong_type/equality.hpp`.


* <A name="equality_with"/>`strong::equality_with<Ts...>` provides operators
  `==` and `!=` between the  strong type and each of the types `Ts...`.
  Note! While `Ts` can include other strong types, it can not refer to the
  strong type being defined. Use [`strong::equality`](#equality) for that.

  Available in `strong_type/equality_with.hpp`.


* <A name="formattable"/>`strong::formattable` adds `std::format` and/or
  [`fmt::format`](https://fmt.dev/latest/index.html) capability, based on
  availability of the formatting library. This can further be controlled
  (globally) with the defines `STRONG_HAS_STD_FORMAT` respectively
  `STRONG_HAS_FMT_FORMAT`. With 0 to disable the support completly, and with 1
  to force the support, disable the auto detection.

  `fmt::format` allows formatting also types that are
  [`strong::ostreamable`](#ostreamable).

  Available in `strong_type/formattable.hpp`.


* <A name="hashable"/>`strong::hashable` allows `std::hash<>` on the type
  (forwards to the underlying type,) to allow use in `std::unordered_set<>` and
  `std::unordered_map<>`.

  Available in `strong_type/hashable.hpp`.


* <A name="implicitly_convertible_to"/>`strong::implicitly_convertible_to<Ts...>`
  provides an `operator Ts() const` for each type `Ts`, providing the underlying
  type supports it.

  Available in `strong_type/implicitly_convertible_to.hpp`.


* <A name="incrementable"/>`strong::incrementable`. Provides
  [`operator++](https://en.cppreference.com/w/cpp/language/operator_incdec) for
  the strong type, using the operator of the underlying type.

  Available in `strong_type/incrementable.hpp`


* <A name="indexed"/>`strong::indexed<D>` allows use of the subscript operator[]
  on type `D`. This also allows member function `at(D)`, providing the
  underlying type supports it. A lame version `indexed<>` allows subscript on
  any type that works.

  Available in `strong_type/indexed.hpp`.


* <A name="iostreamable"/>`strong::iostreamable`. Both
  [`strong::istreamable`](#istreamable) and
  [`strong::ostreamable`](#ostreamable).

  Available in `strong_type/iostreamable.hpp`


* <A name="istreamable"/>`strong::istreamable`. Provides the default
  [`istream`](https://en.cppreference.com/w/cpp/io/basic_istream) extraction
  `operator>>` for the strong type, as handled by the underfying type. Provide
  your own operator istead if you prefer a custom istream extraction operator.

  Available in `strong_type/istreamable.hpp`


* <A name="iterator"/>`strong::iterator` adds functionality needed depending on
  iterator category. If the iterator type is a `random_access_iterator`,
  the strong type is [`strong::indexed<>`](#indexed) and
  [`strong::affine_point<difference>`](#affine_point). It should be
  possible to specify the index type and affine_point type.

  The type trait
  [`std::iterator_traits`](https://en.cppreference.com/w/cpp/iterator/iterator_traits)
  mirrors  the traits of the underlying iterator type.

  Available in `strong_type/iterator.hpp`


* <A name="ordered"/>`strong::ordered` provides operators '<', '<=', '>=' and
  '>'. The strong type offers the same ordering relatin as the underlying type.

  Available in `strong_type/ordered.hpp`


* <A name="ordered_with"/>`strong::ordered_with<Ts...>` provides operators '<',
  '<=', '>=' and '>' between the strong type and each of the types `Ts...`.
   Note! While `Ts` can include other strong types, it cannot refer to the strong
  type being defined. Use [`strong::ordered`](#ordered) for that.

  Available in `strong_type/ordered_with.hpp`


* <A name="ostreamable"/>`strong::ostreamable`. Provides the default
  [`ostream`](https://en.cppreference.com/w/cpp/io/basic_ostream) insertion
  `operator<<` for the strong type, as handled by the underlying type. Provide
  your own operator instead if you prefer a custom  ostream insertion operator.

  Available in `strong_type/ostreamable.hpp`


* <A name="pointer"/>`strong::pointer` allows `operator*` and `operator->`, and
  comparisons with `nullptr` providing the underlying type supports it.

  Available in `strong_type/pointer.hpp`


* <A name="range"/>`strong::range` adds the functionality needed to iterate over
  the elements. The [iterator types](#iterator) are using the same tag as using
  in the range. Only implements types `iterator` and `const_iterator`, and thus
  `.begin()`, `.end()`, `.cbegin()`, `.cend()`, `.begin() const` and
  `.end() const`.

  Available in `strong_type/range.hpp`


* <A name="regular"/>`strong::regular`. Same as [`semiregular`](#semiregular)
  and also equality comparable. A good default base for most types.

  Available in `strong´type/regular.hpp`


* <A name="scalable_with"/>`strong::scalable_with<Ts...>` Allows multiplying and
  dividing the value with each type `Ts`, providing the underlying type supports
  it. It also allows dividing instances of `scalable_with<>`, if the underlying
  type supports it, and returns the first type in the list of `Ts...`.

  Available in `strong_type/scalable_with.hpp`


* <A name="semiregular"/>`strong::semiregular`. This gives you default
  constructible, move/copy constructible, move/copy assignable and swappable.
  A decent default for many types.

  Available in `strong_type/semiregular.hpp`.


* <A name="unique"/>`strong::unique`. Make the type move constructible and move
  assignable but not copy constructible nor copy assignable.

  Available in `strong_type/unique.hpp`


# <A name="utilities"/> Utilities:

A number of small utilities are available directly in `strong_type/type.hpp`.

* `strong::type` provides a non-member `swap()` function as a friend, which
   swaps underlying values using.
  
* `strong::underlying_type<Type>` is `T` for `strong::type<T, Tag, Ms...>` and
   public descendants, and `Type` for other types. 
      
* `strong::uninitialized` can be used to construct instances of `strong::type<T...>`
  without initializing the value. This is only possible if the underlying type
  is [`trivially default constructible`](
  https://en.cppreference.com/w/cpp/language/default_constructor), for example:
  ```C++
  void init(int*);
  void function() {
      strong::type<int, struct int_tag> x(strong::uninitialized);
      // x will have an unspecified value
      init(&value_of(x)); // hopefully the init() function assigns a value
  }
  ```
  
* `strong::type_is<type, modifier>` is a constexpr predicate to test if a type
  has a modifier. For variadic modifiers, like `strong::ordered_with<Ts...>`,
  it tests each of the types `Ts` individually. Example:

  ```C++
  using handle = strong::type<int, struct handle_, strong::regular>;
  
  static_assert(strong::type_is<handle, strong::equality>);
  static_assert(strong::type_is<handle, strong::regular>);
  
  using id = strong::type<int, struct id_, strong::ordered_with<int, long>>;
  
  static_assert(strong::type_is<id, strong::ordered_with<int, long>>);   
  static_assert(strong::type_is<id, strong::ordered_with<long>>);   
  static_assert(strong::type_is<id, strong::ordered_with<int>>);   
  static_assert(strong::type_is<id, strong::ordered_with<>>);   
  ``` 
  
  All `static_assert`s above pass.
   
# <A name="selftest"/> Self test

To build the self-test program(s):

```bash
cmake <strong_type_dir> -DSTRONG_TYPE_UNIT_TEST=yes
cmake --build .
```

This will produce the test programs `self_test`, and conditionally also
`test_fmt8` and `test_fmt9`, depending on which version(s) of
[`{fmt}`](https://fmt.dev/latest/index.html)

N.B. Microsoft Visual Studio MSVC compiler < 19.22 does not handle `constexpr`
correctly. Those found to cause trouble are disabled for those versions.

## <A name="other"/> Other libraries:
 
| Library                                             | Author |
|-----------------------------------------------------|-------------------|
| [type_safe](https://github.com/foonathan/type_safe) | Jonathan Müller   |
| [NamedType](https://github.com/joboccara/NamedType) | Jonathan Boccara  |
| [strong_typedef](https://github.com/anthonywilliams/strong_typedef) | Anthony Williams (justsoftwaresolutions) |

## <A name="presentations"/> Presentations about defining and using strong types

|   |   |
|---|---|
| [![Strong Types for Strong Interfaces](https://img.youtube.com/vi/WVleZqzTw2k/mqdefault.jpg)](https://img.youtube.com/vi/WVleZqzTw2k/mqdefault.jpg) | Jonathan Boccara from MeetingC++ 2017 |
| [![Strong Types in C++](https://img.youtube.com/vi/fWcnp7Bulc8/mqdefault.jpg)](https://youtu.be/fWcnp7Bulc8) | Barney Dellar from C++OnSea 2019 |
| [![Type Safe C++? - LOL! - ;-)](https://img.youtube.com/vi/SWHvNvY-PHw/mqdefault.jpg)](https://youtu.be/SWHvNvY-PHw) | Björn Fahller from ACCU 2018 |
| [![Curiously Coupled Types](https://img.youtube.com/vi/msi4WNQZyWs/mqdefault.jpg)](https://youtu.be/msi4WNQZyWs) | Adi Shavit & Björn Fahller from NDC{Oslo} 2019 |

Discussions, pull-requests, flames are welcome.

[@bjorn_fahller](https://twitter.com/bjorn_fahller)
