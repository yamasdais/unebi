#pragma once

#include <concepts>
#include <optional>

#include <unebi/fwd/chars.hpp>
#include <unebi/util.hpp>

namespace unebi::detail {

template <class... Tester>
struct ChartestMultiImpl;

template <class Tester>
struct ChartestMultiImpl<Tester> {
    using tail_t = std::tuple<ChartestMultiImpl<Tester>>;

    constexpr ChartestMultiImpl(Tester const& tester)
    : tester{tester}
    {}

    template <std::integral T>
        requires std::predicate<Tester, T>
    constexpr bool operator()(T v) const noexcept(noexcept(std::is_nothrow_invocable_v<Tester, T>)) {
        return tester(v);
    }

    constexpr std::optional<ChartestMultiImpl<Tester>> next() const noexcept {
        return {};
    }

    Tester tester;
};

template <class Tester, class... T>
struct ChartestMultiImpl<Tester, T...> {
    using next_type = ChartestMultiImpl<T...>;
    using tail_t = decltype(std::tuple_cat(std::declval<std::tuple<ChartestMultiImpl<Tester, T...>>>(), std::declval<typename next_type::tail_t>()));
    constexpr ChartestMultiImpl(Tester const& tester, T const&... tail)
    : tester{tester}
    , tail{tail...}
    {
    }

    template <std::integral ValueT>
        requires std::predicate<Tester, ValueT>
    constexpr bool operator()(ValueT v) const noexcept(noexcept(std::is_nothrow_invocable_v<Tester, ValueT>)) {
        return tester(v);
    }

    constexpr std::optional<next_type> next() const noexcept {
        return {tail};
    }

    Tester tester;
    next_type tail;
};

} // namespace unebi::detail