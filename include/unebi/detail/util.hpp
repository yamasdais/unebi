#pragma once

#include <concepts>
#include <functional>

namespace unebi::detail {

template <class... Args>
struct TypeHolderImpl {};

template <std::size_t I, class T>
struct GetParamTypeImpl {};

template <template <class...> class T, class Head, class... Tail>
struct GetParamTypeImpl<0u, T<Head, Tail...>> {
    using type = Head;
};

template <template <class...> class T, class Head, class... Tail>
struct GetParamTypeImpl<0u, T<Head const, Tail...>> {
    using type = Head const;
};

template <std::size_t I, template <class...> class T, class Head, class... Tail>
struct GetParamTypeImpl<I, T<Head, Tail...>> {
    using type = typename GetParamTypeImpl<I - 1u, TypeHolderImpl<Tail...>>::type;
};

template <template <class...> class To, class From>
struct CopyParamTypesImpl;

template <template <class...> class To, template <class...> class From,
    class... Args>
struct CopyParamTypesImpl<To, From<Args...>> {
    using type = To<Args...>;
};

} // namespace unebi::detail