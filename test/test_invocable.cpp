#include "catch2.hpp"

#include <strong_type/invocable.hpp>

namespace {
using type = strong::type<int (*)(int), struct type_, strong::invocable>;

constexpr int noexcept_func(int x) noexcept { return x + 1; }
constexpr int except_func(int x) { return x - 1; }
constexpr int &identity(int &x){ return x; }

constexpr type wontthrow{noexcept_func};
constexpr type maythrow(except_func);
using itype = strong::type<int &(*)(int &), struct type_, strong::invocable>;
constexpr itype ident(identity);

template<typename T>
constexpr const T &as_const(const T &t) { return t; }

}

static_assert(wontthrow(0) == 1,"");
static_assert(maythrow(1) == 0,"");
static_assert(as_const(wontthrow)(0) == 1,"");
static_assert(as_const(maythrow)(1) == 0,"");
static_assert(std::move(wontthrow)(0) == 1,"");
static_assert(std::move(maythrow)(1) == 0,"");
static_assert(std::move(as_const(wontthrow))(0) == 1,"");
static_assert(std::move(as_const(maythrow))(1) == 0,"");

int global_invoke_x = 0;
static_assert(std::is_same<int&, decltype(ident(global_invoke_x))>{}, "");
static_assert(std::is_same<int&, decltype(std::move(ident)(global_invoke_x))>{}, "");
static_assert(std::is_same<int&, decltype(as_const(ident)(global_invoke_x))>{}, "");
static_assert(std::is_same<int&, decltype(std::move(as_const(ident))(global_invoke_x))>{}, "");

TEST_CASE("invocable qualifiers are respected")
{
    struct S {
        int operator()() & {return 1;}
        int operator()() && { return 2;}
        int operator()() const & { return 3;}
        int operator()() const && { return 4;}
    };

    using T = strong::type<S, struct T_, strong::invocable>;

    T t(S{});
    REQUIRE(t() == 1);
    REQUIRE(as_const(t)() == 3);
    REQUIRE(std::move(t)() == 2);
    REQUIRE(std::move(as_const(t))() == 4);
}