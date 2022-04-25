#pragma once

#include <concepts>
#include <functional>

#include <unebi/detail/util.hpp>

namespace unebi {

template <class T, std::size_t I>
using get_tparam_t = typename detail::GetParamTypeImpl<I, std::remove_cvref_t<T>>::type;

template <template <class...> class To, class From>
using copy_tparam_t = typename detail::CopyParamTypesImpl<To, std::remove_cvref_t<From>>::type;

template <class ...>
struct dbg_t;

}