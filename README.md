# strong_type
An experimental C++ stong typedef-ish thingie

Highly experimental, for now.

Public domain. May change to something else if experimentation proves
successful.

Very much inspired by [`@foonathan`](https://twitter.com/foonathan)
[`type_safe`](https://github.com/foonathan/type_safe) library, but aim is
slightly different. Limit scope for type safety only. No runtime checks. Also
strive for a higher level abstraciton of the needed functionality. The idea
is to suffer no runtime penalty, but to capture misuse at compile time
(for example accidentally subtracting from a handle, or swapping to parameters
in a functiion call) while still being easy to use for inexperienced
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
type `myint`, you can't pass it an instance of `otherint`, and vise-versa. You
also can't cross-assign, cross-create or cross-compare.

To access more functionality, you add modifiers. For example:

```Cpp
using ordered_int = strong::type<int, ordered_int_, strong::ordered>;
```

Type `ordered_int` now supports relational order comparisons, like `<`,
(provided the underlying type, int this case `int`, does.) Type `ordered_int`
can thus be used as key in `std::map`<> or `std::set<>`.

Other modifiers are:

* `strong::ostreamable`, `strong::istreamable`, strong::iostreamable`, which
  provides the default iostreams integration (as handled by the underlying
  type.) Provide your own operators instead if you prefer that.

* `strong::up_counter`, `strong::down_counter`, `strong::bidirectional_counter`.
  Supports `operator++` and `operator--`. *not* happy about these, especially
  the names. Suggestions for improvemests welcome.

* `strong::boolean` provides `explicit operator bool() const`, providing the
  underlying type supports it.

* `strong::hashable` allows `std::hash<>` on the type (forwards to the
  underlying type,) to allow use in `std::unordered_set<>` and
  `std::unordered_map<>`

* `strong::difference<D>` allows instances to be subtracted (yielding a `D`) or
  to add or subtract a `D` to an instance. Think pointer and `ptrdiff_t`. It's
  also possible to use a lamer `strong::difference<>`, which allows a delta of
  any type that works. I think this functionality is OK, but I'm not happy with
  the name, and maybe the ability to subtract instances should be separate from
  the ability to add/subtract deltas?

* `strong::pointer` allows `operator*` and `operator->`, providing the
  underlyin type supports it.

* `strong::unique` is movable but not copyable. Think `std::unique_ptr<>` as
  an example.

* `strong::scoped` is not movable and certainly not copyable.

* `strong::arithmetic` allows addition, subtraction, multiplication, division
  and remainter of instances.

* `strong::bitarithmetic` allows bitwise `&`, bitwise `|`, bitwise `^` and
  shift opetations.

* `strong::indexed<D>` allows use af the subscript operator[] on type `D`.
  This also allows member function `at(D)`, providing the underlying type
  supports it. A lame verison `indexed<>` allows subscript on any type that
  works.


For modifier `strong::arithmetic` the type trait `std::is_arithmetic<>` is true.


Which are the /right/ modifiers to have? Going into too fine detail makes no
sense and becomes a burden. `iterator`? One for each iterator category?

To build the self test program:

`cmake <strong_type_dir> -DCMAKE_BUILD_TYPE=Debug`
`cmake --build . --target self_test`

Discussions, pull-requests, flames are welcome.

[@bjorn_fahller](https://twitter.com/bjorn_fahller)
