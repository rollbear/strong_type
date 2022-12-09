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


// include first to ensure there aren't any unmet header dependencies
#include <strong_type/strong_type.hpp>

#include <iomanip>
#include <unordered_set>
#include <memory>
#include <algorithm>
#include <vector>
#include <sstream>

#if CATCH2 == 3
#include <catch2/catch_test_macros.hpp>
#else
#include <catch2/catch.hpp>
#endif
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

template <typename T, typename U>
using multiply = decltype(std::declval<const T&>() * std::declval<const U&>());

template <typename T, typename U>
using divide = decltype(std::declval<const T&>() / std::declval<const U&>());

template <typename T, typename U>
using modulo = decltype(std::declval<const T&>() % std::declval<const U&>());

template <typename R, typename I>
using at = decltype(std::declval<const R&>().at(std::declval<I>()));

template <typename T>
using begin_type = decltype(std::declval<T&>().begin());

template <typename T>
using end_type = decltype(std::declval<T&>().begin());

template <typename T>
using ostream_insertion = decltype(std::declval<std::ostream&>() << std::declval<const T&>());

template <typename T>
using istream_extraction = decltype(std::declval<std::istream&>() >> std::declval<T&>());

template <typename T>
using preincrement = decltype(++std::declval<T&>());

template <typename T>
using predecrement = decltype(--std::declval<T&>());

template <typename T1, typename T2>
using swapping = decltype(swap(std::declval<T1&>(), std::declval<T2&>()));

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

template <typename T, typename U = T>
using is_divisible = is_detected<divide, T, U>;

template <typename T, typename U = T>
using is_modulo_able = is_detected<modulo, T, U>;

template <typename T, typename U = T>
using is_multipliable = is_detected<multiply, T, U>;

template <typename T, typename U>
using is_indexable = is_detected<indexing, T, U>;

template <typename T>
using is_range = std::integral_constant<bool, is_detected<begin_type, T>::value && is_detected<end_type, T>::value>;

template <typename T>
using is_nothrow_swappable = std::integral_constant<bool, noexcept(swap(std::declval<T&>(), std::declval<T&>()))>;

template <typename T1, typename T2>
using is_strong_swappable_with = is_detected<swapping, T1, T2>;

template <typename R, typename I>
using has_at = is_detected<at, R, I>;

using uint = unsigned int;

using handle = strong::type<int, struct handle_tag>;

static_assert(std::is_same<int, strong::underlying_type_t<handle>>{},"");
static_assert(std::is_same<int, strong::underlying_type_t<int>>{},"");

static_assert(std::is_lvalue_reference<decltype(value_of(std::declval<handle&>()))>{},"");
static_assert(std::is_lvalue_reference<decltype(value_of(std::declval<const handle&>()))>{},"");
static_assert(std::is_const<std::remove_reference_t<decltype(value_of(std::declval<const handle&>()))>>{},"");
static_assert(std::is_rvalue_reference<decltype(value_of(std::declval<handle&&>()))>{},"");

static_assert(!std::is_default_constructible<handle>{},"");
static_assert(std::is_nothrow_constructible<handle, int&&>{},"");
static_assert(std::is_nothrow_constructible<handle, const int&>{},"");
static_assert(std::is_copy_constructible<handle>{},"");
static_assert(std::is_nothrow_constructible<handle, strong::uninitialized_t>{}, "");
static_assert(is_nothrow_swappable<handle>{},"");
static_assert(!is_equal_comparable<handle>{}, "");
static_assert(std::is_nothrow_assignable<handle, const handle&>{}, "");
static_assert(std::is_nothrow_assignable<handle, handle&&>{}, "");
static_assert(!is_less_than_comparable<handle>{},"");
static_assert(!is_ostreamable<handle>{},"");
static_assert(!is_istreamable<handle>{},"");
static_assert(!std::is_constructible<bool, handle>{}, "");
static_assert(!is_incrementable<handle>{},"");
static_assert(!is_decrementable<handle>{},"");
static_assert(!is_hashable<handle>{},"");
static_assert(!is_indexable<handle, int>{}, "");
static_assert(!is_range<handle>{}, "");

using handle2 = strong::type<int, struct handle2_tag>;

static_assert(!std::is_constructible<handle, const handle2&>{},"");
static_assert(!std::is_constructible<handle, handle2&&>{},"");
static_assert(!std::is_assignable<handle, const handle2&>{},"");
static_assert(!std::is_assignable<handle, handle2&&>{},"");
static_assert(!is_equal_comparable<handle, handle2>{},"");

using bhandle = strong::type<int, struct bhandle_tag, strong::boolean>;

static_assert(!std::is_default_constructible<bhandle>{},"");
static_assert(std::is_nothrow_constructible<bhandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<bhandle, const int&>{},"");
static_assert(std::is_copy_constructible<bhandle>{},"");
static_assert(is_nothrow_swappable<bhandle>{},"");
static_assert(!is_equal_comparable<bhandle>{}, "");
static_assert(std::is_nothrow_assignable<bhandle, const bhandle&>{}, "");
static_assert(std::is_nothrow_assignable<bhandle, bhandle&&>{}, "");
static_assert(!is_less_than_comparable<bhandle>{},"");
static_assert(!is_ostreamable<bhandle>{},"");
static_assert(!is_istreamable<bhandle>{},"");
static_assert(std::is_constructible<bool, bhandle>{}, "");
static_assert(!is_incrementable<bhandle>{},"");
static_assert(!is_decrementable<bhandle>{},"");
static_assert(!is_hashable<bhandle>{},"");
static_assert(!is_indexable<bhandle, int>{}, "");
static_assert(!is_range<bhandle>{}, "");

using dchandle = strong::type<int, struct dchandle_tag, strong::default_constructible>;

static_assert(std::is_default_constructible<dchandle>{},"");
static_assert(std::is_nothrow_constructible<dchandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<dchandle, const int&>{},"");
static_assert(std::is_copy_constructible<dchandle>{},"");
static_assert(is_nothrow_swappable<dchandle>{},"");
static_assert(!is_equal_comparable<dchandle>{}, "");
static_assert(std::is_nothrow_assignable<dchandle, const dchandle&>{}, "");
static_assert(std::is_nothrow_assignable<dchandle, dchandle&&>{}, "");
static_assert(!is_less_than_comparable<dchandle>{},"");
static_assert(!is_ostreamable<dchandle>{},"");
static_assert(!is_istreamable<dchandle>{},"");
static_assert(!std::is_constructible<bool, dchandle>{}, "");
static_assert(!is_incrementable<dchandle>{},"");
static_assert(!is_decrementable<dchandle>{},"");
static_assert(!is_hashable<dchandle>{},"");
static_assert(!is_indexable<dchandle, int>{}, "");
static_assert(!is_range<dchandle>{}, "");

using ahandle = strong::type<int, struct ahandle_tag, strong::arithmetic>;

static_assert(!std::is_default_constructible<ahandle>{},"");
static_assert(std::is_nothrow_constructible<ahandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<ahandle, const int&>{},"");
static_assert(std::is_copy_constructible<ahandle>{},"");
static_assert(is_nothrow_swappable<ahandle>{},"");
static_assert(!is_equal_comparable<ahandle>{}, "");
static_assert(std::is_nothrow_assignable<ahandle, const ahandle&>{}, "");
static_assert(std::is_nothrow_assignable<ahandle, ahandle&&>{}, "");
static_assert(!is_less_than_comparable<ahandle>{},"");
static_assert(!is_ostreamable<ahandle>{},"");
static_assert(!is_istreamable<ahandle>{},"");
static_assert(!std::is_constructible<bool, ahandle>{}, "");
static_assert(!is_incrementable<ahandle>{},"");
static_assert(!is_decrementable<ahandle>{},"");
static_assert(is_modulo_able<ahandle>{},"");
static_assert(!is_hashable<ahandle>{},"");
static_assert(is_subtractable<ahandle>{}, "");
static_assert(!is_subtractable<ahandle, bhandle>{},"");
static_assert(!is_indexable<ahandle, int>{}, "");
static_assert(!is_range<ahandle>{}, "");

using ahandle2 = strong::type<int, struct ahandle2_tag, strong::arithmetic>;

static_assert(!is_subtractable<ahandle, ahandle2>{},"");

using afhandle = strong::type<float, struct afloat_tag, strong::arithmetic>;
static_assert(!is_modulo_able<afhandle>{},"");

using hhandle = strong::type<int, struct hhandle_tag, strong::hashable>;

static_assert(!std::is_default_constructible<hhandle>{},"");
static_assert(std::is_nothrow_constructible<hhandle, int&&>{},"");
static_assert(std::is_nothrow_constructible<hhandle, const int&>{},"");
static_assert(std::is_copy_constructible<hhandle>{},"");
static_assert(is_nothrow_swappable<hhandle>{},"");
static_assert(!is_equal_comparable<hhandle>{}, "");
static_assert(std::is_nothrow_assignable<hhandle, const hhandle&>{}, "");
static_assert(std::is_nothrow_assignable<hhandle, hhandle&&>{}, "");
static_assert(!is_less_than_comparable<hhandle>{},"");
static_assert(!is_ostreamable<hhandle>{},"");
static_assert(!is_istreamable<hhandle>{},"");
static_assert(!std::is_constructible<bool, hhandle>{}, "");
static_assert(!is_incrementable<hhandle>{},"");
static_assert(!is_decrementable<hhandle>{},"");
static_assert(is_hashable<hhandle>{},"");
static_assert(!is_indexable<hhandle, int>{}, "");
static_assert(!is_range<hhandle>{}, "");

using ihandle = strong::type<std::string, struct string_tag, strong::indexed<uint>>;

static_assert(!std::is_default_constructible<ihandle>{},"");
static_assert(!std::is_nothrow_constructible<ihandle, uint>{},"");
static_assert(std::is_copy_constructible<ihandle>{},"");
static_assert(!std::is_constructible<ihandle, strong::uninitialized_t>{}, "");

static_assert(is_nothrow_swappable<ihandle>{},"");
static_assert(!is_equal_comparable<ihandle>{}, "");
static_assert(!std::is_nothrow_assignable<ihandle, const ihandle&>{}, "");
static_assert(std::is_nothrow_assignable<ihandle, ihandle&&>{}, "");
static_assert(!is_less_than_comparable<ihandle>{},"");
static_assert(!is_ostreamable<ihandle>{},"");
static_assert(!is_istreamable<ihandle>{},"");
static_assert(!std::is_constructible<bool, ihandle>{}, "");
static_assert(!is_incrementable<ihandle>{},"");
static_assert(!is_decrementable<ihandle>{},"");
static_assert(!is_hashable<ihandle>{},"");
static_assert(is_indexable<ihandle, uint>{}, "");
static_assert(has_at<ihandle, uint>{}, "");
static_assert(!is_range<ihandle>{}, "");

using dhandle = strong::type<int, struct int_tag, strong::affine_point<handle>>;

static_assert(!std::is_default_constructible<dhandle>{},"");
static_assert(std::is_nothrow_constructible<dhandle, int>{},"");
static_assert(std::is_copy_constructible<dhandle>{},"");
static_assert(is_nothrow_swappable<dhandle>{},"");
static_assert(!is_equal_comparable<dhandle>{}, "");
static_assert(std::is_nothrow_assignable<dhandle, const dhandle&>{}, "");
static_assert(std::is_nothrow_assignable<dhandle, dhandle&&>{}, "");
static_assert(!is_less_than_comparable<dhandle>{},"");
static_assert(!is_ostreamable<dhandle>{},"");
static_assert(!is_istreamable<dhandle>{},"");
static_assert(!std::is_constructible<bool, dhandle>{}, "");
static_assert(!is_incrementable<dhandle>{},"");
static_assert(!is_decrementable<dhandle>{},"");
static_assert(!is_hashable<dhandle>{},"");
static_assert(!is_indexable<dhandle, int>{}, "");
static_assert(is_subtractable<dhandle,handle>{}, "");
static_assert(is_subtractable<dhandle,dhandle>{},"");
static_assert(!is_addable<dhandle,dhandle>{},"");
static_assert(is_addable<dhandle,handle>{},"");
static_assert(is_addable<handle,dhandle>{},"");
static_assert(!is_range<dhandle>{}, "");

using ddhandle = strong::type<int, struct int_tag, strong::affine_point<>>;
static_assert(std::is_same<ddhandle::difference, strong::type<int, struct int_tag, strong::difference>>{}, "");

using ri = strong::type<int*, struct ipt, strong::iterator>;

static_assert(!std::is_default_constructible<ri>{},"");
static_assert(std::is_nothrow_constructible<ri, int*>{},"");
static_assert(std::is_copy_constructible<ri>{},"");
static_assert(is_equal_comparable<ri>{}, "");
static_assert(std::is_nothrow_assignable<ri, const ri&>{}, "");
static_assert(std::is_nothrow_assignable<ri, ri&&>{}, "");
static_assert(is_less_than_comparable<ri>{},"");
static_assert(!is_ostreamable<ri>{},"");
static_assert(!is_istreamable<ri>{},"");
static_assert(!std::is_constructible<bool, ri>{}, "");
static_assert(is_incrementable<ri>{},"");
static_assert(is_decrementable<ri>{},"");
static_assert(!is_hashable<ri>{},"");
static_assert(is_indexable<ri, int>{}, "");
static_assert(is_subtractable<ri,ri>{}, "");
static_assert(is_subtractable<ri,int>{},"");
static_assert(!is_addable<ri,ri>{},"");
static_assert(is_addable<ri,int>{},"");
static_assert(is_addable<int,ri>{},"");
static_assert(!is_range<ri>{}, "");

using li = strong::type<std::unordered_set<int>::iterator, struct lit, strong::iterator>;

static_assert(!std::is_default_constructible<li>{},"");
static_assert(std::is_copy_constructible<li>{},"");
static_assert(is_equal_comparable<li>{}, "");
static_assert(std::is_nothrow_assignable<li, const li&>{}, "");
static_assert(std::is_nothrow_assignable<li, li&&>{}, "");
static_assert(!is_less_than_comparable<li>{},"");
static_assert(!is_ostreamable<li>{},"");
static_assert(!is_istreamable<li>{},"");
static_assert(!std::is_constructible<bool, li>{}, "");
static_assert(is_incrementable<li>{},"");
#ifndef _MSC_VER
static_assert(!is_decrementable<li>{},"");
#endif
static_assert(!is_hashable<li>{},"");
static_assert(!is_indexable<li, int>{}, "");
static_assert(!is_subtractable<li,li>{}, "");
static_assert(!is_subtractable<li,int>{},"");
static_assert(!is_addable<li,li>{},"");
static_assert(!is_addable<li,int>{},"");
static_assert(!is_addable<int,li>{},"");
static_assert(!is_range<li>{}, "");

using rhandle = strong::type<std::vector<int>, struct r_tag, strong::range>;

static_assert(!std::is_default_constructible<rhandle>{},"");
static_assert(std::is_copy_constructible<rhandle>{},"");
static_assert(!is_equal_comparable<rhandle>{}, "");
static_assert(!std::is_nothrow_assignable<rhandle, const rhandle&>{}, "");
static_assert(std::is_nothrow_assignable<rhandle, rhandle&&>{}, "");
static_assert(!is_less_than_comparable<rhandle>{},"");
static_assert(!is_ostreamable<rhandle>{},"");
static_assert(!is_istreamable<rhandle>{},"");
static_assert(!std::is_constructible<bool, rhandle>{}, "");
static_assert(!is_incrementable<rhandle>{},"");
static_assert(!is_decrementable<rhandle>{},"");
static_assert(!is_hashable<rhandle>{},"");
static_assert(!is_indexable<rhandle, int>{}, "");
static_assert(!is_subtractable<rhandle,rhandle>{}, "");
static_assert(!is_subtractable<rhandle,int>{},"");
static_assert(!is_addable<rhandle,rhandle>{},"");
static_assert(!is_addable<rhandle,int>{},"");
static_assert(!is_addable<rhandle,li>{},"");
static_assert(is_range<rhandle>{}, "");
static_assert(is_range<const rhandle>{}, "");

using rhi = rhandle::iterator;
static_assert(std::is_nothrow_copy_constructible<rhi>{},"");
static_assert(std::is_nothrow_move_constructible<rhi>{},"");
static_assert(!std::is_default_constructible<rhi>{},"");
static_assert(std::is_nothrow_copy_assignable<rhi>{},"");
static_assert(std::is_nothrow_move_assignable<rhi>{},"");
static_assert(std::is_nothrow_destructible<rhi>{},"");

using crhi = rhandle::const_iterator;
static_assert(std::is_nothrow_copy_constructible<crhi>{},"");
static_assert(std::is_nothrow_move_constructible<crhi>{},"");
static_assert(!std::is_default_constructible<crhi>{},"");
static_assert(std::is_nothrow_copy_assignable<crhi>{},"");
static_assert(std::is_nothrow_move_assignable<crhi>{},"");
static_assert(std::is_nothrow_destructible<crhi>{},"");

using uhandle = strong::type<int, struct uh_, strong::difference>;
static_assert(!std::is_default_constructible<uhandle>{},"");
static_assert(std::is_copy_constructible<uhandle>{},"");
static_assert(is_equal_comparable<uhandle>{}, "");
static_assert(std::is_nothrow_assignable<uhandle, const uhandle&>{}, "");
static_assert(std::is_nothrow_assignable<uhandle, uhandle&&>{}, "");
static_assert(is_less_than_comparable<uhandle>{},"");
static_assert(!is_ostreamable<uhandle>{},"");
static_assert(!is_istreamable<uhandle>{},"");
static_assert(!std::is_constructible<bool, uhandle>{}, "");
static_assert(!is_incrementable<uhandle>{},"");
static_assert(!is_decrementable<uhandle>{},"");
static_assert(!is_hashable<uhandle>{},"");
static_assert(!is_indexable<uhandle, int>{}, "");
static_assert(is_subtractable<uhandle,uhandle>{}, "");
static_assert(!is_subtractable<uhandle,int>{},"");
static_assert(is_addable<uhandle,uhandle>{},"");
static_assert(!is_addable<uhandle,int>{},"");
static_assert(!is_addable<int,uhandle>{},"");
static_assert(is_divisible<uhandle, uhandle>{}, "");
static_assert(is_divisible<uhandle, int>{}, "");
static_assert(is_multipliable<uhandle, int>{}, "");
static_assert(is_multipliable<int, uhandle>{}, "");
static_assert(!is_multipliable<uhandle, uhandle>{}, "");
static_assert(is_modulo_able<uhandle>{},"");
static_assert(is_modulo_able<uhandle, int>{},"");
static_assert(!is_addable<uhandle,li>{},"");
static_assert(!is_range<uhandle>{}, "");
static_assert(!is_range<const uhandle>{}, "");

using ufhandle = strong::type<float, struct uf_, strong::difference>;
static_assert(!is_modulo_able<ufhandle>{},"");
static_assert(!is_modulo_able<ufhandle, float>{},"");

static_assert(is_strong_swappable_with<handle, handle>::value, "");
static_assert(!is_strong_swappable_with<handle, handle2>::value, "");
static_assert(!is_strong_swappable_with<
                      strong::type<int, struct handle_tag>,
                      strong::type<long, struct handle_tag>
                  >::value, "");
static_assert(!is_strong_swappable_with<
                      strong::type<int, struct handle_tag>,
                      strong::type<int, struct handle_tag, strong::default_constructible>
                  >::value, "");
static_assert(!is_strong_swappable_with<
                      strong::type<int, struct handle_tag>,
                      strong::type<int, struct handle_tag, strong::incrementable>
                  >::value, "");

using ehandle = strong::type<int, struct handle_tag, strong::equality>;
static_assert(is_equal_comparable<ehandle>{}, "");
using rehandle = strong::type<std::vector<int>, struct r_tag,
                              strong::range, strong::equality>;
static_assert(is_equal_comparable<rehandle>{}, "");

using srint = strong::type<int, struct srint_, strong::semiregular>;
static_assert(std::is_nothrow_default_constructible<srint>{},"");
static_assert(std::is_nothrow_copy_constructible<srint>{},"");
static_assert(std::is_nothrow_move_constructible<srint>{},"");
static_assert(std::is_nothrow_copy_assignable<srint>{},"");
static_assert(std::is_nothrow_move_assignable<srint>{},"");
static_assert(is_strong_swappable_with<srint,srint>{},"");
static_assert(!is_equal_comparable<srint>{},"");

using regint = strong::type<int, struct rint_, strong::regular>;
static_assert(std::is_nothrow_default_constructible<regint>{},"");
static_assert(std::is_nothrow_copy_constructible<regint>{},"");
static_assert(std::is_nothrow_move_constructible<regint>{},"");
static_assert(std::is_nothrow_copy_assignable<regint>{},"");
static_assert(std::is_nothrow_move_assignable<regint>{},"");
static_assert(is_strong_swappable_with<regint,regint>{},"");
static_assert(is_equal_comparable<regint>{},"");

using ibde = strong::type<int, struct ibdre_, strong::convertible_to<bool,double>>;
static_assert(!std::is_convertible<ibde, bool>{},"");
static_assert(!std::is_convertible<ibde, double>{},"");
static_assert(!std::is_convertible<ibde, float>{},"");
static_assert(std::is_nothrow_constructible<bool, ibde>{},"");
static_assert(std::is_nothrow_constructible<double, ibde>{},"");
static_assert(!std::is_constructible<float, ibde>{},"");

using ibdi = strong::type<int, struct ibdri_, strong::implicitly_convertible_to<bool,double>>;
static_assert(std::is_convertible<ibdi, bool>{},"");
static_assert(std::is_convertible<ibdi, double>{},"");
static_assert(!std::is_convertible<ibdi, float>{},"");
static_assert(std::is_nothrow_constructible<bool, ibdi>{},"");
static_assert(std::is_nothrow_constructible<double, ibdi>{},"");
static_assert(!std::is_constructible<float, ibdi>{},"");

using ssbi = strong::type<const char*, struct ssi_, strong::implicitly_convertible_to<std::string, bool>>;
static_assert(std::is_convertible<ssbi, std::string>{}, "");
static_assert(std::is_convertible<ssbi, bool>{}, "");
static_assert(std::is_constructible<std::string, ssbi>{}, "");
#ifndef _MSC_VER
static_assert(!std::is_nothrow_constructible<std::string, ssbi>{}, "");
#endif
static_assert(std::is_nothrow_constructible<bool, ssbi>{}, "");

using ui = strong::type<int, struct ui_, strong::unique>;
static_assert(std::is_nothrow_constructible<ui, int>{},"");
static_assert(!std::is_copy_constructible<ui>{},"");
static_assert(std::is_nothrow_move_constructible<ui>{},"");
static_assert(!std::is_copy_assignable<ui>{},"");
static_assert(std::is_nothrow_move_assignable<ui>{},"");

using us = strong::type<std::string, struct us_, strong::unique>;
static_assert(std::is_constructible<us, const char*>{},"");
static_assert(!std::is_nothrow_constructible<us, const char*>{},"");
static_assert(!std::is_copy_constructible<us>{},"");
static_assert(std::is_nothrow_move_constructible<us>{},"");
static_assert(!std::is_copy_assignable<us>{},"");
static_assert(std::is_nothrow_move_assignable<us>{},"");

struct seqv : strong::type<std::string, seqv, strong::equality_with<const char*,us>>
{};

static_assert(!is_equal_comparable<seqv>{},"");
static_assert(is_equal_comparable<seqv, const char*>{},"");
static_assert(is_equal_comparable<const char*, seqv>{},"");
static_assert(is_equal_comparable<seqv, us>{},"");
static_assert(is_equal_comparable<us, seqv>{},"");
static_assert(!is_equal_comparable<seqv, std::string>{},"");
static_assert(!is_equal_comparable<std::string, seqv>{},"");

struct iov : strong::type<int, iov, strong::ordered_with<int>>
{
  using strong::type<int, iov, strong::ordered_with<int>>::type;
};

static_assert(std::is_nothrow_constructible<iov, int>{},"");
static_assert(std::is_nothrow_copy_constructible<iov>{},"");
static_assert(std::is_nothrow_move_constructible<iov>{},"");
static_assert(std::is_nothrow_copy_assignable<iov>{},"");
static_assert(std::is_nothrow_move_assignable<iov>{},"");
static_assert(is_less_than_comparable<iov, int>{},"");
static_assert(is_less_than_comparable<int, iov>{},"");
static_assert(!is_less_than_comparable<iov, iov>{},"");

template <typename T>
const T& as_const(T& t) { return t;}
template <typename T>
const T&& as_const(T&& t) { return std::move(t);}

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
  strong::type<std::unique_ptr<int>, struct p_, strong::pointer> p(std::move(orig));
  REQUIRE(*p == 3);
  REQUIRE(!orig);
  REQUIRE(value_of(p).get() == addr);
}

TEST_CASE("value can be copy constructed if member can")
{
  using ptr = strong::type<std::shared_ptr<int>, struct p_, strong::pointer>;
  ptr orig{std::make_shared<int>(3)};
  ptr p{orig};
  REQUIRE(value_of(orig).use_count() == 2);
  REQUIRE(*p == 3);
}

TEST_CASE("value can be copy assigned")
{
  using ptr = strong::type<std::shared_ptr<int>, struct p_, strong::pointer>;
  ptr orig{std::make_shared<int>(3)};
  ptr p{nullptr};
  p = orig;
  REQUIRE(value_of(orig).use_count() == 2);
  REQUIRE(*p == 3);
}

TEST_CASE("value can be move assigned")
{
  using ptr = strong::type<std::unique_ptr<int>, struct p_, strong::pointer>;
  ptr orig{std::make_unique<int>(3)};
  ptr p{nullptr};
  p = std::move(orig);
  REQUIRE(orig == nullptr);
  REQUIRE(*p == 3);
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

TEST_CASE("values can be compared using operator==")
{
  using type = strong::type<int, struct i_, strong::equality>;

  type i{3};
  type j{4};
  type k{3};
  REQUIRE_FALSE(i == j);
  REQUIRE(i == k);
}

TEST_CASE("values can be compared using operator!=")
{
  using type = strong::type<int, struct i_, strong::equality>;

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
                STATIC_REQUIRE(std::is_same<decltype(v), const int&>{});
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
                STATIC_REQUIRE(std::is_same<decltype(v), const int&>{});
            }
            AND_THEN("the value is the one constructed from")
            {
                REQUIRE(v == 3);
            }
        }
    }
}

TEST_CASE("equality_with can compare with defined type")
{
    using t1 = strong::type<int, struct t1_>;
    using t2 = strong::type<int, struct t2_, strong::equality_with<t1>>;

    REQUIRE(t1{1} == t2{1});
    REQUIRE(t2{1} == t1{1});
    REQUIRE(t1{1} != t2{2});
    REQUIRE(t2{2} != t1{1});
}

TEST_CASE("strong::unique is movable")
{
    using t = strong::type<std::unique_ptr<int>, struct t_, strong::unique>;

    GIVEN("an initialized source")
    {
        t source{std::make_unique<int>(3)};
        auto* addr = value_of(source).get();

        WHEN("move constructing a dest")
        {
            auto dest = std::move(source);
            THEN("the source is moved from")
            {
                REQUIRE(value_of(source).get() == nullptr);
            }
            AND_THEN("the dest has the value that source had")
            {
                REQUIRE(*value_of(dest) == 3);
                REQUIRE(value_of(dest).get() == addr);
            }
        }
        AND_WHEN("move assigning to another instance")
        {
            t dest{std::make_unique<int>(4)};
            dest = std::move(source);
            THEN("the source is moved from")
            {
                REQUIRE(value_of(source).get() == nullptr);
            }
            AND_THEN("the dest has the value that source had")
            {
                REQUIRE(*value_of(dest) == 3);
                REQUIRE(value_of(dest).get() == addr);
            }
        }
    }
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
  REQUIRE(value_of(i) == 3);
  REQUIRE(value_of(j) == 4);
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

TEST_CASE("an incrementable can be incremented")
{
  using C = strong::type<int, struct i_, strong::incrementable>;

  C c{3};
  REQUIRE(value_of(++c) == 4);
  REQUIRE(value_of(c) == 4);
  REQUIRE(value_of(c++) == 4);
  REQUIRE(value_of(c) == 5); }

TEST_CASE("a decrementable can be decremented")
{
  using C = strong::type<int, struct i_, strong::decrementable>;

  C c{10};
  REQUIRE(value_of(--c) == 9);
  REQUIRE(value_of(c) == 9);
  REQUIRE(value_of(c--) == 9);
  REQUIRE(value_of(c) == 8);
}

TEST_CASE("a bicrementable goes both ways")
{
  using C = strong::type<int, struct i_, strong::bicrementable>;

  C c{3};
  REQUIRE(value_of(++c) == 4);
  REQUIRE(value_of(c) == 4);
  REQUIRE(value_of(c++) == 4);
  REQUIRE(value_of(c) == 5);
  REQUIRE(value_of(--c) == 4);
  REQUIRE(value_of(c) == 4);
  REQUIRE(value_of(c--) == 4);
  REQUIRE(value_of(c) == 3);
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

  P p{nullptr};
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
  REQUIRE(value_of(i + j) == 7);
  REQUIRE(value_of(i) == 3);
  REQUIRE(value_of(j) == 4);
  i += j;
  REQUIRE(value_of(i) == 7);
  REQUIRE(value_of(j) == 4);
}

TEST_CASE("arithmetic types can be subtracted")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{7};
  T j{4};
  REQUIRE(value_of(i - j) == 3);
  REQUIRE(value_of(i) == 7);
  REQUIRE(value_of(j) == 4);
  i -= j;
  REQUIRE(value_of(i) == 3);
  REQUIRE(value_of(j) == 4);
}

TEST_CASE("arithmetic types can be multiplied")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{12};
  T j{3};
  REQUIRE(value_of(i * j) == 36);
  REQUIRE(value_of(i) == 12);
  REQUIRE(value_of(j) == 3);
  i *= j;
  REQUIRE(value_of(i) == 36);
  REQUIRE(value_of(j) == 3);
}

TEST_CASE("arithmetic types can be divided")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{12};
  T j{3};
  REQUIRE(value_of(i / j) == 4);
  REQUIRE(value_of(i) == 12);
  REQUIRE(value_of(j) == 3);
  i /= j;
  REQUIRE(value_of(i) == 4);
  REQUIRE(value_of(j) == 3);
}

TEST_CASE("the remainder of integral arithmetic types can be calculated")
{
    using T = strong::type<int, struct i_, strong::arithmetic>;

    T i{12};
    T j{5};
    REQUIRE(value_of(i % j) == 2);
    REQUIRE(value_of(i) == 12);
    REQUIRE(value_of(j) == 5);
    i %= j;
    REQUIRE(value_of(i) == 2);
    REQUIRE(value_of(j) == 5);
}

TEST_CASE("an aritmmetic type can be negated")
{
  using T = strong::type<int, struct i_, strong::arithmetic>;

  T i{12};
  REQUIRE(value_of(-i) == -12);
  REQUIRE(value_of(i) == 12);
}

TEST_CASE("bitarithmetic types can be bitanded")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};
  T j{u(0x5555)};

  REQUIRE(value_of(i & j) == u(0x0505));
  REQUIRE(value_of(i) == u(0x0f0f));
  REQUIRE(value_of(j) == u(0x5555));
  REQUIRE(value_of(i &= j) == u(0x0505));
  REQUIRE(value_of(i) == u(0x0505));
  REQUIRE(value_of(j) == u(0x5555));
}

TEST_CASE("bitarithmetic types can be bitored")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};
  T j{u(0x5555)};

  REQUIRE(value_of(i | j) == u(0x5f5f));
  REQUIRE(value_of(i) == u(0x0f0f));
  REQUIRE(value_of(j) == u(0x5555));
  REQUIRE(value_of(i |= j) == u(0x5f5f));
  REQUIRE(value_of(i) == u(0x5f5f));
  REQUIRE(value_of(j) == u(0x5555));
}

TEST_CASE("bitarithmetic types can be bitxored")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};
  T j{u(0x5555)};

  REQUIRE(value_of(i ^ j) == u(0x5a5a));
  REQUIRE(value_of(i) == u(0x0f0f));
  REQUIRE(value_of(j) == u(0x5555));
  REQUIRE(value_of(i ^= j) == u(0x5a5a));
  REQUIRE(value_of(i) == u(0x5a5a));
  REQUIRE(value_of(j) == u(0x5555));
}

TEST_CASE("bitarithmetic types can be left shifted")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};

  REQUIRE(value_of(i << 2) == u(0x3c3c));
  REQUIRE(value_of(i) == u(0x0f0f));
  REQUIRE(value_of(i <<= 3) == u(0x7878));
  REQUIRE(value_of(i) == u(0x7878));
}

TEST_CASE("bitarithmetic types can be right shifted")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};

  REQUIRE(value_of(i >> 2) == u(0x03c3));
  REQUIRE(value_of(i) == u(0x0f0f));
  REQUIRE(value_of(i >>= 1) == u(0x0787));
  REQUIRE(value_of(i) == u(0x0787));
}

TEST_CASE("bitarithmetic types can be inverted")
{
  using T = strong::type<uint16_t, struct s_, strong::bitarithmetic>;
  using u = uint16_t;

  T i{u(0x0f0f)};

  REQUIRE(value_of(~i) == u(0xf0f0));
  REQUIRE(value_of(i) == u(0x0f0f));
}

TEST_CASE("indexed can be accessed using operator [] and .at()")
{
  using I = strong::type<unsigned, struct I_>;
  using T = strong::type<std::string, struct s_, strong::indexed<>>;
  using TI = strong::type<std::string, struct s_, strong::indexed<I>>;

  T s("foo");
  const T c("bar");
  TI si("foo");
  const TI ci("bar");

  auto& r = s[I{0U}];
  static_assert(!std::is_const<std::remove_reference_t<decltype(r)>>{}, "");
  REQUIRE(r == 'f');
  auto& ri = si[I{0U}];
  static_assert(!std::is_const<std::remove_reference_t<decltype(ri)>>{}, "");
  REQUIRE(ri == 'f');
  auto& cr = c[I{0U}];
  static_assert(std::is_const<std::remove_reference_t<decltype(cr)>>{}, "");
  REQUIRE(cr == 'b');
  auto& cri = ci[I{0U}];
  static_assert(std::is_const<std::remove_reference_t<decltype(cri)>>{}, "");
  REQUIRE(cri == 'b');

  auto& ar = s.at(I{0U});
  static_assert(!std::is_const<std::remove_reference_t<decltype(ar)>>{}, "");
  REQUIRE(ar == 'f');
  auto& ari = si.at(I{0U});
  static_assert(!std::is_const<std::remove_reference_t<decltype(ari)>>{}, "");
  REQUIRE(ari == 'f');
  auto& acr = c.at(I{0U});
  static_assert(std::is_const<std::remove_reference_t<decltype(acr)>>{}, "");
  REQUIRE(acr == 'b');
  auto& acri = ci.at(I{0U});
  static_assert(std::is_const<std::remove_reference_t<decltype(acri)>>{}, "");
  REQUIRE(acri == 'b');

  // operator[] on std::string returns lvalue reference
  static_assert(std::is_same<decltype(std::declval<std::string&&>()[1]), char&>{},"");
  // hence ...

  auto&& tmp = std::move(s)[1U];
  STATIC_REQUIRE(std::is_same<decltype(tmp), char&>{});
  REQUIRE(tmp == 'o');

  auto&& tmpat = std::move(s).at(1U);
  STATIC_REQUIRE(std::is_same<decltype(tmpat), char&>{});
  REQUIRE(tmpat == 'o');


  auto&& tmpi = std::move(si)[I{1U}];
  STATIC_REQUIRE(std::is_same<decltype(tmpi), char&>{});
  REQUIRE(tmpi == 'o');

  auto&& tmpiat = std::move(si).at(I{1U});
  STATIC_REQUIRE(std::is_same<decltype(tmpiat), char&>{});
  REQUIRE(tmpiat == 'o');
}

template <typename T, size_t N>
class simple_array
{
public:
    T& operator[](int x) & { return data_[x];}
    T&& operator[](int x) && { return std::move(data_[x]);}
    const T& operator[](int x) const & { return data_[x];}
    const T& operator[](int x) const && { return data_[x];}
private:
    T data_[N] = {};
};

template <typename T, size_t N>
using array = strong::type<simple_array<T, N>, struct array_,
                           strong::default_constructible,
                           strong::indexed<int>>;
template <typename ...> struct S;
TEST_CASE("indexed can be accessed with operator[] without at()")
{
    static_assert(is_indexable<array<int, 3>, int>{}, "");
    static_assert(!has_at<array<int, 3>, int>{}, "");

    using A = array<int, 3>;

    A a{};
    const A ca{};

    static_assert(std::is_same<decltype(a[1]), int&>{}, "");
    static_assert(std::is_same<decltype(ca[1]), const int&>{}, "");
    static_assert(std::is_same<decltype(std::move(a)[1]), int&&>{}, "");
    static_assert(std::is_same<decltype(std::move(ca)[1]), const int&>{}, "");

    a[0] = 3;
    a[1] = 4;
    a[2] = 5;

    REQUIRE(as_const(a)[0] == 3);
    REQUIRE(a[1] == 4);
    REQUIRE(std::move(as_const(a))[2] == 5);
}

TEST_CASE("affine_point types can be subtracted")
{
  using D = strong::type<int, struct i_>;
  using T = strong::type<int, struct i_, strong::affine_point<D>>;

  T t1{3};
  T t2{8};
  auto d = t2 - t1;
  static_assert(std::is_same<decltype(d), D>{}, "");
  REQUIRE(value_of(d) == 5);
}

TEST_CASE("affine point type with defaulted difference type yields it when subtracting")
{
    using T = strong::type<int, struct i_, strong::affine_point<>>;

    T t1{3};
    T t2{8};
    auto d = t2 - t1;
    static_assert(std::is_same<decltype(d), strong::type<int, struct i_, strong::difference>>{},"");
    REQUIRE(value_of(d) == 5);
}

TEST_CASE("affine_point types can be added with the delta type")
{
  using D = strong::type<int, struct i_>;
  using T = strong::type<int, struct i_, strong::affine_point<D>>;

  T t1{8};
  D d{3};

  auto t2 = t1 + d;
  static_assert(std::is_same<decltype(t2), T>{}, "");
  REQUIRE(value_of(t2) == 11);
  auto t3 = d + t1;
  static_assert(std::is_same<decltype(t3), T>{}, "");
  REQUIRE(value_of(t3) == 11);
  t1 += d;
  REQUIRE(value_of(t1) == 11);
}

TEST_CASE("affine_point types can be subtracted with the delta type")
{
  using D = strong::type<int, struct i_>;
  using T = strong::type<int, struct i_, strong::affine_point<D>>;

  T t1{8};
  D d{3};

  auto t2 = t1 - d;
  static_assert(std::is_same<decltype(t2), T>{}, "");
  REQUIRE(value_of(t2) == 5);
  t1 -= d;
  REQUIRE(value_of(t1) == 5);
}

TEST_CASE("adding difference types yields a difference type")
{
  using U = strong::type<int, struct u_, strong::difference>;

  U u1{3};
  U u2{4};

  auto r = u1 + u2;
  static_assert(std::is_same<decltype(r), U>{},"");
  REQUIRE(value_of(r) == 7);
}

TEST_CASE("subtracting difference types yields a difference type")
{
  using U = strong::type<int, struct U_, strong::difference>;

  U u1{8};
  U u2{3};

  auto r = u1 - u2;

  static_assert(std::is_same<decltype(r), U>{}, "");
  REQUIRE(value_of(r) == 5);
}

TEST_CASE("dividing difference types yields a base type")
{
  using U = strong::type<int, struct U_, strong::difference>;

  U u1{8};
  U u2{2};

  auto r = u1/u2;
  static_assert(std::is_same<decltype(r), int>{}, "");
  REQUIRE(r == 4);
}

TEST_CASE("remainder after division between difference types yields a base type")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u1{15};
    U u2{6};

    auto r = u1 % u2;

    static_assert(std::is_same<decltype(r), int>{},"");
    REQUIRE(r == 3);
}

TEST_CASE("remainder after division between difference type and base type yields a difference type")
{
    using U = strong::type<int, struct U_, strong::difference, strong::regular>;

    U u{15};

    auto r = u % 6;

    static_assert(std::is_same<decltype(r), U>{}, "");
    REQUIRE(r == U{3});
}

TEST_CASE("dividing a difference type with its base type yields a difference")
{
  using U = strong::type<int, struct U_, strong::difference>;

  U u{8};

  auto r = u/2;
  static_assert(std::is_same<decltype(r), U>{}, "");
  REQUIRE(value_of(r) == 4);
}

TEST_CASE("difference types are ordered and equality comparable")
{
    using U = strong::type<int, struct U_, strong::difference>;

    U u1{1};
    U u2{2};
    U u3{1};

    REQUIRE(u1 == u3);
    REQUIRE(!(u1 == u2));
    REQUIRE(u1 != u2);
    REQUIRE(!(u1 != u3));
    REQUIRE(u1 < u2);
    REQUIRE(!(u1 < u3));
    REQUIRE(u1 <= u2);
    REQUIRE(u1 <= u3);
    REQUIRE(!(u2 <= u1));
    REQUIRE(u2 > u1);
    REQUIRE(!(u3 > u1));
    REQUIRE(!(u3 > u2));
    REQUIRE(u3 >= u1);
    REQUIRE(u3 >= u1);
    REQUIRE(!(u3 >= u2));
}

TEST_CASE("multiplying a difference with its base type yields a difference")
{
  using U = strong::type<int, struct U_, strong::difference>;

  U u{3};

  auto r1 = u * 2;
  static_assert(std::is_same<decltype(r1), U>{}, "");
  REQUIRE(value_of(r1) == 6);

  auto r2 = 3 * u;
  static_assert(std::is_same<decltype(r2), U>{}, "");
  REQUIRE(value_of(r2) == 9);
}

TEST_CASE("iterators work with algorithms")
{
  std::unordered_set<int> is{3,2,8,4,11,9,22,23};
  using si = strong::type<std::unordered_set<int>::iterator, struct si_, strong::iterator>;
  si sb{is.begin()};
  si se{is.end()};

  std::vector<int> v;
  std::copy(sb, se, std::back_inserter(v));

  using vi = strong::type<std::vector<int>::iterator, struct vi_, strong::iterator>;

  vi vb{v.begin()};
  vi ve{v.end()};
  std::sort(vb, ve);
  REQUIRE(vb[0] == 2);
  REQUIRE(vb[7] == 23);
}

TEST_CASE("a range can be used with range based for")
{
  using iv = strong::type<std::vector<int>, struct vi_, strong::range>;

  iv v{3,2,1};
  int n = 3;
  for (auto& e : v)
  {
    REQUIRE(e-- == n--);
  }
  auto& cv = v;
  n = 2;
  for (auto& e : cv)
  {
    REQUIRE(e == n--);
  }
}

TEST_CASE("iterator type can be used from range")
{
  using iv = strong::type<std::vector<int>, struct vi_, strong::range>;

  iv v{3,2,1};
  int n = 3;
  for (iv::iterator i{v.begin()}; i != v.end(); ++i)
  {
    REQUIRE((*i)-- == n--);
  }
  const auto& cv = v;
  n = 2;
  for (iv::const_iterator i{cv.begin()}; i != cv.end(); ++i)
  {
    REQUIRE(*i == n--);
  }
  n = 2;
  for (iv::const_iterator i{v.cbegin()}; i != v.cend(); ++i)
  {
    REQUIRE(*i == n--);
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

TEST_CASE("conversions")
{
  using ssbe = strong::type<const char*, struct ssbe_, strong::convertible_to<bool, std::string>>;

  ssbi validi{"value"};
  ssbi invalidi(nullptr);
  ssbe valide("value");
  ssbe invalide(nullptr);

  REQUIRE(validi);
  REQUIRE(valide);
  REQUIRE_FALSE(invalidi);
  REQUIRE_FALSE(invalide);

  std::string svalidi = validi;
  REQUIRE(svalidi == "value");
#if !defined(__GNUC__) || __GNUC__ > 7
  std::string svalide{valide};
  REQUIRE(svalide == "value");
#endif
}

TEST_CASE("ordered_with")
{
  const iov i1{1};

  REQUIRE(i1 < 2);
  REQUIRE(0 < i1);
  REQUIRE_FALSE(i1 < 1);
  REQUIRE_FALSE(1 < i1);
  REQUIRE(i1 <= 1);
  REQUIRE(i1 <= 2);
  REQUIRE_FALSE(i1 <= 0);
  REQUIRE(1 <= i1);
  REQUIRE(0 <= i1);
  REQUIRE_FALSE(2<= i1);
  REQUIRE(i1 >= 1);
  REQUIRE(i1 >= 0);
  REQUIRE_FALSE(i1 >= 2);
  REQUIRE(1 >= i1);
  REQUIRE(2 >= i1);
  REQUIRE_FALSE(0 >= i1);
  REQUIRE(i1 > 0);
  REQUIRE_FALSE(i1 > 1);
  REQUIRE(2 > i1);
  REQUIRE_FALSE(1 > i1);
}

TEST_CASE("strong::hashable in unordered_set")
{
  using hs = strong::type<std::string, struct hs_, strong::hashable, strong::regular>;

  using sset = std::unordered_set<hs>;

  sset strings{hs{"foo"}, hs{"bar"}};

  REQUIRE(strings.find(hs{"foo"}) != strings.end());
  REQUIRE(strings.find(hs{"bar"}) != strings.end());
  REQUIRE(strings.find(hs{"baz"}) == strings.end());
}
#if STRONG_HAS_STD_FORMAT
TEST_CASE("format")
{
  using formatint = strong::type<int, struct formattag, strong::formattable>;

  formatint fi{5};
  CHECK(std::format("{:d}", fi) == std::format("{:d}", 5));
  // Use std::vformat to mitigate the compile time check.
  CHECK_THROWS_AS(std::vformat("{:s}", std::make_format_args(fi)), std::format_error);
}
#endif
