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

 #include "catch2.hpp"

// include first to ensure there aren't any unmet header dependencies
#include "strong_type/strong_type.hpp"

#if !defined(STRONG_TYPE_IMPORT_STD_LIBRARY)
#include <iomanip>
#include <unordered_set>
#include <memory>
#include <algorithm>
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

static_assert(!std::numeric_limits<handle>::is_specialized, "");
#if defined(__cpp_concepts)
static_assert(!std::is_base_of<std::numeric_limits<void>, std::numeric_limits<handle>>{},"");
#else
static_assert(std::is_base_of<std::numeric_limits<void>, std::numeric_limits<handle>>{},"");
#endif
static_assert(std::is_same<int, strong::underlying_type_t<handle>>{},"");
static_assert(std::is_same<int, strong::underlying_type_t<int>>{},"");

static_assert(!strong::type_is_v<handle, strong::regular>, "");
static_assert(!strong::type_is_v<handle, strong::semiregular>, "");
static_assert(!strong::type_is_v<handle, strong::default_constructible>, "");


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
static_assert(!is_divisible<handle>{}, "");

using handle2 = strong::type<int, struct handle2_tag>;

static_assert(!std::numeric_limits<handle2>::is_specialized, "");
static_assert(!std::is_constructible<handle, const handle2&>{},"");
static_assert(!std::is_constructible<handle, handle2&&>{},"");
static_assert(!std::is_assignable<handle, const handle2&>{},"");
static_assert(!std::is_assignable<handle, handle2&&>{},"");
static_assert(!is_equal_comparable<handle, handle2>{},"");
static_assert(!is_divisible<handle2>{}, "");

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
static_assert(!is_divisible<bhandle>{}, "");

using dchandle = strong::type<int, struct dchandle_tag, strong::default_constructible>;

static_assert(!std::numeric_limits<dchandle>::is_specialized, "");
static_assert(strong::type_is_v<dchandle, strong::default_constructible>, "");
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
static_assert(!is_divisible<dchandle>{}, "");

using ahandle = strong::type<int, struct ahandle_tag, strong::arithmetic>;

static_assert(std::numeric_limits<ahandle>::is_specialized, "");
static_assert(value_of(std::numeric_limits<ahandle>::min()) == std::numeric_limits<int>::min(),"");
static_assert(value_of(std::numeric_limits<ahandle>::max()) == std::numeric_limits<int>::max(),"");
static_assert(strong::type_is_v<ahandle, strong::arithmetic>, "");
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
static_assert(is_divisible<ahandle>{}, "");

using ahandle2 = strong::type<int, struct ahandle2_tag, strong::arithmetic>;

static_assert(!is_subtractable<ahandle, ahandle2>{},"");

using afhandle = strong::type<float, struct afloat_tag, strong::arithmetic>;
static_assert(std::numeric_limits<afhandle>::is_specialized, "");
static_assert(value_of(std::numeric_limits<afhandle>::min()) == std::numeric_limits<float>::min(), "");
static_assert(value_of(std::numeric_limits<afhandle>::max()) == std::numeric_limits<float>::max(), "");
static_assert(value_of(std::numeric_limits<afhandle>::lowest()) == std::numeric_limits<float>::lowest(), "");
static_assert(value_of(std::numeric_limits<afhandle>::denorm_min()) == std::numeric_limits<float>::denorm_min(), "");
static_assert(std::numeric_limits<afhandle>::min_exponent10 == std::numeric_limits<float>::min_exponent10, "");

static_assert(!is_modulo_able<afhandle>{},"");


using hhandle = strong::type<int, struct hhandle_tag, strong::hashable>;

static_assert(strong::type_is_v<hhandle, strong::hashable>, "");
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

static_assert(strong::type_is_v<ihandle, strong::indexed<uint>>, "");
static_assert(!strong::type_is_v<ihandle, strong::indexed<>>, "");
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

static_assert(strong::type_is_v<dhandle, strong::affine_point<handle>>, "");
static_assert(!strong::type_is_v<dhandle, strong::affine_point<>>, "");

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
static_assert(strong::type_is_v<ddhandle, strong::affine_point<>>, "");

using ri = strong::type<int*, struct ipt, strong::iterator>;
static_assert(strong::type_is_v<ri, strong::iterator>, "");
static_assert(strong::type_is_v<ri, strong::equality>, "");
static_assert(strong::type_is_v<ri, strong::pointer>, "");
static_assert(strong::type_is_v<ri, strong::affine_point<std::intptr_t>>, "");
//static_assert(!strong::type_is_v<ri, strong::regular>, "");
//static_assert(!std::is_default_constructible<ri>{},"");
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
static_assert(strong::type_is_v<li, strong::iterator>, "");
static_assert(strong::type_is_v<li, strong::pointer>, "");
static_assert(strong::type_is_v<li, strong::equality>, "");
static_assert(!strong::type_is_v<li, strong::affine_point<>>, "");
//static_assert(!std::is_default_constructible<li>{},"");
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
static_assert(strong::type_is_v<rhi, strong::iterator>, "");
static_assert(strong::type_is_v<rhi, strong::pointer>, "");
static_assert(strong::type_is_v<rhi, strong::equality>, "");
static_assert(strong::type_is_v<rhi, strong::affine_point<std::intptr_t>>, "");
static_assert(std::is_nothrow_copy_constructible<rhi>{},"");
static_assert(std::is_nothrow_move_constructible<rhi>{},"");
//static_assert(!std::is_default_constructible<rhi>{},"");
static_assert(std::is_nothrow_copy_assignable<rhi>{},"");
static_assert(std::is_nothrow_move_assignable<rhi>{},"");
static_assert(std::is_nothrow_destructible<rhi>{},"");

using crhi = rhandle::const_iterator;
static_assert(strong::type_is_v<crhi, strong::iterator>, "");
static_assert(strong::type_is_v<crhi, strong::pointer>, "");
static_assert(strong::type_is_v<crhi, strong::equality>, "");
static_assert(strong::type_is_v<crhi, strong::affine_point<std::intptr_t>>, "");
static_assert(std::is_nothrow_copy_constructible<crhi>{},"");
static_assert(std::is_nothrow_move_constructible<crhi>{},"");
//static_assert(!std::is_default_constructible<crhi>{},"");
static_assert(std::is_nothrow_copy_assignable<crhi>{},"");
static_assert(std::is_nothrow_move_assignable<crhi>{},"");
static_assert(std::is_nothrow_destructible<crhi>{},"");
static_assert(!is_divisible<crhi>{}, "");
using uhandle = strong::type<int, struct uh_, strong::difference>;
static_assert(strong::type_is_v<uhandle, strong::difference>, "");
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
static_assert(strong::type_is_v<ufhandle, strong::difference>, "");
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

static_assert(strong::type_is_v<ehandle, strong::equality>, "");
static_assert(is_equal_comparable<ehandle>{}, "");
using rehandle = strong::type<std::vector<int>, struct r_tag,
                              strong::range, strong::equality>;
static_assert(is_equal_comparable<rehandle>{}, "");

using srint = strong::type<int, struct srint_, strong::semiregular>;

static_assert(strong::type_is_v<srint, strong::semiregular>, "");
static_assert(!strong::type_is_v<srint, strong::regular>, "");
static_assert(!strong::type_is_v<srint, strong::equality>, "");
static_assert(std::is_nothrow_default_constructible<srint>{},"");
static_assert(std::is_nothrow_copy_constructible<srint>{},"");
static_assert(std::is_nothrow_move_constructible<srint>{},"");
static_assert(std::is_nothrow_copy_assignable<srint>{},"");
static_assert(std::is_nothrow_move_assignable<srint>{},"");
static_assert(is_strong_swappable_with<srint,srint>{},"");
static_assert(!is_equal_comparable<srint>{},"");

using regint = strong::type<int, struct rint_, strong::regular>;

static_assert(strong::type_is_v<regint, strong::regular>, "");
static_assert(strong::type_is_v<regint, strong::semiregular>, "");
static_assert(strong::type_is_v<regint, strong::equality>, "");
static_assert(std::is_nothrow_default_constructible<regint>{},"");
static_assert(std::is_nothrow_copy_constructible<regint>{},"");
static_assert(std::is_nothrow_move_constructible<regint>{},"");
static_assert(std::is_nothrow_copy_assignable<regint>{},"");
static_assert(std::is_nothrow_move_assignable<regint>{},"");
static_assert(is_strong_swappable_with<regint,regint>{},"");
static_assert(is_equal_comparable<regint>{},"");

using ibde = strong::type<int, struct ibdre_, strong::convertible_to<bool,double>>;
static_assert(strong::type_is_v<ibde, strong::convertible_to<bool, double>>, "");
static_assert(strong::type_is_v<ibde, strong::convertible_to<double, bool>>, "");
static_assert(strong::type_is_v<ibde, strong::convertible_to<bool>>, "");
static_assert(strong::type_is_v<ibde, strong::convertible_to<double>>, "");
static_assert(strong::type_is_v<ibde, strong::convertible_to<>>, "");
static_assert(!std::is_convertible<ibde, bool>{},"");
static_assert(!std::is_convertible<ibde, double>{},"");
static_assert(!std::is_convertible<ibde, float>{},"");
static_assert(std::is_nothrow_constructible<bool, ibde>{},"");
static_assert(std::is_nothrow_constructible<double, ibde>{},"");
static_assert(!std::is_constructible<float, ibde>{},"");

using ibdi = strong::type<int, struct ibdri_, strong::implicitly_convertible_to<bool,double>>;
static_assert(strong::type_is_v<ibdi, strong::implicitly_convertible_to<bool, double>>, "");
static_assert(strong::type_is_v<ibdi, strong::implicitly_convertible_to<double, bool>>, "");
static_assert(strong::type_is_v<ibdi, strong::implicitly_convertible_to<bool>>, "");
static_assert(strong::type_is_v<ibdi, strong::implicitly_convertible_to<double>>, "");
static_assert(strong::type_is_v<ibdi, strong::implicitly_convertible_to<>>, "");
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

static_assert(strong::type_is_v<ui, strong::unique>, "");
static_assert(std::is_nothrow_constructible<ui, int>{},"");
static_assert(!std::is_copy_constructible<ui>{},"");
static_assert(std::is_nothrow_move_constructible<ui>{},"");
static_assert(!std::is_copy_assignable<ui>{},"");
static_assert(std::is_nothrow_move_assignable<ui>{},"");

using us = strong::type<std::string, struct us_, strong::unique>;
static_assert(strong::type_is_v<us, strong::unique>, "");
static_assert(std::is_constructible<us, const char*>{},"");
static_assert(!std::is_nothrow_constructible<us, const char*>{},"");
static_assert(!std::is_copy_constructible<us>{},"");
static_assert(std::is_nothrow_move_constructible<us>{},"");
static_assert(!std::is_copy_assignable<us>{},"");
static_assert(std::is_nothrow_move_assignable<us>{},"");

struct seqv : strong::type<std::string, seqv, strong::equality_with<const char*,us>>
{};
static_assert(strong::type_is_v<seqv, strong::equality_with<const char*, us>>, "");
static_assert(strong::type_is_v<seqv, strong::equality_with<us, const char*>>, "");
static_assert(strong::type_is_v<seqv, strong::equality_with<us>>, "");
static_assert(strong::type_is_v<seqv, strong::equality_with<const char*>>, "");
static_assert(strong::type_is_v<seqv, strong::equality_with<>>, "");
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
static_assert(strong::type_is_v<iov, strong::ordered_with<int>>, "");
static_assert(strong::type_is_v<iov, strong::ordered_with<>>, "");
static_assert(std::is_nothrow_constructible<iov, int>{},"");
static_assert(std::is_nothrow_copy_constructible<iov>{},"");
static_assert(std::is_nothrow_move_constructible<iov>{},"");
static_assert(std::is_nothrow_copy_assignable<iov>{},"");
static_assert(std::is_nothrow_move_assignable<iov>{},"");
static_assert(is_less_than_comparable<iov, int>{},"");
static_assert(is_less_than_comparable<int, iov>{},"");
static_assert(!is_less_than_comparable<iov, iov>{},"");

using scild = strong::type<int, struct scild_, strong::scalable_with<int, long, double>>;
using schdi = strong::type<int, struct scldi_, strong::scalable_with<handle, double, int>>;
static_assert(strong::type_is_v<scild, strong::scalable_with<int, long, double>>, "");
static_assert(is_divisible<scild, scild>{},"");
static_assert(is_divisible<scild, int>{},"");
static_assert(is_divisible<scild, long>{},"");
static_assert(is_divisible<scild, double>{},"");
static_assert(!is_divisible<scild, handle>{},"");

static_assert(is_divisible<schdi, schdi>{},"");
static_assert(is_divisible<schdi, handle>{},"");
static_assert(is_divisible<schdi, double>{},"");
static_assert(is_divisible<schdi, int>{},"");
static_assert(std::is_same<int, decltype(scild{3}/scild{2})>{},"");
static_assert(std::is_same<handle, decltype(schdi{3} / schdi{1})>{},"");

#if STRONG_HAS_STD_FORMAT
TEST_CASE("format")
{
  using formatint = strong::type<int, struct formattag, strong::formattable>;

  static_assert(strong::type_is_v<formatint, strong::formattable>);
  formatint fi{5};
  CHECK(std::format("{:d}", fi) == std::format("{:d}", 5));
  // Use std::vformat to mitigate the compile time check.
  CHECK_THROWS_AS(std::vformat("{:s}", std::make_format_args(fi)), std::format_error);
}
#endif
