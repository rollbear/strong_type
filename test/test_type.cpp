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

#include <strong_type/type.hpp>

#include "catch2.hpp"

#include "test_utils.hpp"

#include <memory>


TEST_CASE("default_constructible initializes with underlying default constructor")
{
    constexpr strong::type<int, struct i_, strong::default_constructible> vc;
    STATIC_REQUIRE(value_of(vc) == 0);
    strong::type<int, struct i_, strong::default_constructible> vr;
    REQUIRE(value_of(vr) == 0);
}

TEST_CASE("Construction from a value type lvalue copies it")
{
    auto orig = std::make_shared<int>(3);
    strong::type<std::shared_ptr<int>, struct int_> i{orig};
    REQUIRE(*value_of(i) == 3);
    REQUIRE(orig.use_count() == 2);
    REQUIRE(value_of(i).get() == orig.get());
}

TEST_CASE("Construction from multiple parameters constructs value from them")
{
    strong::type<std::string, struct string_> s(3U, 'a');
    REQUIRE(value_of(s) == "aaa");
}

TEST_CASE("construction from an rvalue reference moves from it")
{
    auto orig = std::make_unique<int>(3);
    auto addr = orig.get();
    strong::type<std::unique_ptr<int>, struct p_> p(std::move(orig));
    REQUIRE(*value_of(p) == 3);
    REQUIRE(!orig);
    REQUIRE(value_of(p).get() == addr);
}

TEST_CASE("value can be copy constructed if member can")
{
    using ptr = strong::type<std::shared_ptr<int>, struct p_>;
    ptr orig{std::make_shared<int>(3)};
    ptr p{orig};
    REQUIRE(value_of(orig).use_count() == 2);
    REQUIRE(*value_of(p) == 3);
}

TEST_CASE("value can be copy assigned")
{
    using ptr = strong::type<std::shared_ptr<int>, struct p_>;
    ptr orig{std::make_shared<int>(3)};
    ptr p{nullptr};
    p = orig;
    REQUIRE(value_of(orig).use_count() == 2);
    REQUIRE(*value_of(p) == 3);
}

TEST_CASE("value can be move assigned")
{
    using ptr = strong::type<std::unique_ptr<int>, struct p_>;
    ptr orig{std::make_unique<int>(3)};
    ptr p{nullptr};
    p = std::move(orig);
    REQUIRE(value_of(orig)== nullptr);
    REQUIRE(*value_of(p) == 3);
}

TEST_CASE("value can be retained from const lvalue ref")
{
    const strong::type<int, struct i_> i{3};
    auto&& r = value_of(i);
    REQUIRE(r == 3);
    static_assert(std::is_const<std::remove_reference_t <decltype(r)>>{},"");
    static_assert(std::is_lvalue_reference<decltype(r)>{},"");
}

TEST_CASE("value can be retained from rvalue ref")
{
    strong::type<int, struct i_> i{3};
    auto&& r = value_of(std::move(i));
    REQUIRE(r == 3);
    static_assert(!std::is_const<std::remove_reference_t <decltype(r)>>{},"");
    static_assert(std::is_rvalue_reference<decltype(r)>{},"");
}

TEST_CASE("freestanding value_of() gets the underlying value")
{
    GIVEN("a strong value")
    {
        using type = strong::type<int, struct type_>;
        type var{3};
        WHEN("calling value_of() on a const lvalue")
        {
            auto &&v = value_of(as_const(var));
            THEN("a const lvalue reference is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), const int&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v ==  3);
            }
        }
        AND_WHEN("calling value_of() on a non-const lvalue")
        {
            auto&& v = value_of(var);
            THEN("a non-const lvalue reference is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), int&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
            AND_THEN("a write to the returned reference changes the value")
            {
                v = 4;
                REQUIRE(value_of(var) == 4);
            }
        }
        AND_WHEN("calling value_of() on a non-const rvalue")
        {
            auto&& v = value_of(std::move(var));
            THEN("a non-const rvalue reference is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), int&&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
        }
        AND_WHEN("calling value_of() on a const rvalue")
        {
            auto&& v = value_of(std::move(as_const(var)));
            THEN("a const lvalue refercence is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), const int&&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
        }
    }
}

TEST_CASE("member value_of() gets the underlying value")
{
    GIVEN("a strong value")
    {
        using type = strong::type<int, struct type_>;
        type var{3};
        WHEN("calling value_of() on a const lvalue")
        {
            auto &&v = as_const(var).value_of();
            THEN("a const lvalue reference is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), const int&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v ==  3);
            }
        }
        AND_WHEN("calling value_of() on a non-const lvalue")
        {
            auto&& v = var.value_of();
            THEN("a non-const lvalue reference is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), int&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
            AND_THEN("a write to the returned reference changes the value")
            {
                v = 4;
                REQUIRE(value_of(var) == 4);
            }
        }
        AND_WHEN("calling value_of() on a non-const rvalue")
        {
            auto&& v = std::move(var).value_of();
            THEN("a non-const rvalue reference is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), int&&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
        }
        AND_WHEN("calling value_of() on a const rvalue")
        {
            auto&& v = std::move(as_const(var)).value_of();
            THEN("a const lvalue refercence is returned")
            {
                STATIC_REQUIRE(std::is_same<decltype(v), const int&&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
        }
    }
}

TEST_CASE("swap")
{
    using strong_int = strong::type<int, struct si_>;

    strong_int v1{6};
    strong_int v2{34};

    swap(v1, v2);

    CHECK(v1.value_of() == 34);
    CHECK(v2.value_of() == 6);
}

#if __cpp_nontype_template_args >= 201911L

template <auto> class foo {};

TEST_CASE("value can be used as non-type template parameter") {

    using strong_int = strong::type<int, struct si_>;
    foo<strong_int{5}> foo;
}

#endif
