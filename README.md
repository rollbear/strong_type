# strong_type
An experimental C++ strong typedef-ish thingie

Highly experimental, for now.

Public domain. May change to something else if experimentation proves
successful.

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
#include <strong_type.hpp>
using myint = strong::type<int, struct my_int_>;
```

`myint` is a very basic handle. You can initialize it. You can do
equal/not-equal comparison with other instances of the same type, and you can
access its underlying `int` instance with `value(variable)`

```Cpp
using otherint = strong::type<int, struct other_int_>;
```

`otherint` is a distinct type from `myint`. If a function takes an argument of
type `myint`, you can't pass it an instance of `otherint`, and vice-versa. You
also can't cross-assign, cross-create or cross-compare.

To access more functionality, you add modifiers. For example:

```Cpp
using ordered_int = strong::type<int, ordered_int_, strong::ordered>;
```

Type `ordered_int` now supports relational order comparisons, like `<`,
(provided the underlying type, int this case `int`, does.) Type `ordered_int`
can thus be used as key in `std::map`<> or `std::set<>`.

Other modifiers are:

* `strong::ostreamable`, `strong::istreamable`, `strong::iostreamable`, which
  provide the default iostream integrations (as handled by the underlying
  type.) Provide your own operators instead if you prefer that.

* `strong::up_counter`, `strong::down_counter`, `strong::bidirectional_counter`.
  Support `operator++` and `operator--`. *not* happy about these, especially
  the names. Suggestions for improvements welcome.

* `strong::boolean` provides `explicit operator bool() const`, providing the
  underlying type supports it.

* `strong::hashable` allows `std::hash<>` on the type (forwards to the
  underlying type,) to allow use in `std::unordered_set<>` and
  `std::unordered_map<>`

* `strong::distance` allows instances to be subtracted and added (yielding a
  `strong::distance`,) divideded (yielding the base type), or multiplied or
  divided with the base type, yielding another `strong::distance`. A
  `strong::distance` is also `strong::ordered`

* `strong::data_point<D>` allows instances to be subtracted (yielding a `D`) or
  to add or subtract a `D` to an instance. Think pointer and `ptrdiff_t`, or
  `std::time_point<>` and `std::duration<>`. It is natural that `D` is
  of a `strong::distance` type. I think this functionality is OK, but I'm not happy
  with the name.

* `strong::pointer` allows `operator*` and `operator->`, and comparisons with
  `nullptr` providing the underlying type supports it.

* `strong::arithmetic` allows addition, subtraction, multiplication, division
  and remainder of instances.

* `strong::bitarithmetic` allows bitwise `&`, bitwise `|`, bitwise `^` and
  shift operations.

* `strong::indexed<D>` allows use of the subscript operator[] on type `D`.
  This also allows member function `at(D)`, providing the underlying type
  supports it. A lame version `indexed<>` allows subscript on any type that
  works.

* `strong::iterator` adds functionality needed depending on iterator category.
  If the iterator type is a `random_access_iterator`, the strong type
  is `strong::indexed<>` and `strong::data_point<difference_type>`. It should be
  possible to specify the index type and data_point type.

* `strong::range` adds the functionality needed to iterate over the elements.
  the iterator types are using the same tag as using in the range. Only
  implements types `iterator` and `const_iterator`, and thus `.begin()`,
  `.end()`, `.cbegin()`, `.cend()`, `.begin() const` and `.end() const`.
  Are reverse iterators important, and thus `rbegin()`, `rend()` and friends?
    
For modifier `strong::arithmetic`, the type trait `std::is_arithmetic<>` is true.

For modifier `strong::iterator`, the type trait `std::iterator_traits` mirrors
the traits of the underlying iterator type.

Which are the *right* modifiers to have? Going into too fine detail makes no
sense and becomes a burden. `iterator`? One for each iterator category?

To build the self-test program:

```bash
cmake <strong_type_dir> -DCMAKE_BUILD_TYPE=Debug
cmake --build . --target self_test
```

N.B. It looks like Microsoft Visual Studio MSVC compiler *really* doesn't like
this code. If you're familier with MSVC idiosyncracies and/or willing to file
bug reports, please lend a hand.

Discussions, pull-requests, flames are welcome.

[@bjorn_fahller](https://twitter.com/bjorn_fahller)
