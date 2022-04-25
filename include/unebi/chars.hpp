#pragma once

#include <functional>
#include <type_traits>
#include <concepts>
#include <tuple>
#include <variant>

#include <unebi/fwd/chars.hpp>
#include <unebi/detail/chars.hpp>

namespace unebi {

// customization point
template <std::integral From, std::integral To>
struct chartest_cast {
    constexpr To operator ()(From const ch) const noexcept {
        return static_cast<To>(ch);
    }
};

template <std::integral Sample, std::integral Op>
struct chartest_common_type {
    using type = std::common_type_t<Sample, Op>;
};


// customization for char
template <std::integral To>
    requires (!std::same_as<To, char>)
struct chartest_cast<char, To> {
    constexpr To operator ()(char const ch) const noexcept {
        return static_cast<To>(static_cast<std::make_unsigned_t<char>>(ch));
    }
};

//
// 
template <
    std::integral Ch
    , template <class...> class Predicate
    >
struct chartest_single {
    constexpr explicit chartest_single(Ch const ch)
    : ch{ch}
    {}

    template <std::integral T>
        requires std::invocable<
            Predicate<typename chartest_common_type<Ch, T>::type>
            , typename chartest_common_type<Ch, T>::type
            , typename chartest_common_type<Ch, T>::type
            >
    constexpr bool operator()(T const val) const noexcept(noexcept(
        Predicate<typename chartest_common_type<Ch, T>::type>{}(val, std::declval<Ch>())
    )) {
        using Common = typename chartest_common_type<T, Ch>::type;
        return Predicate<Common>{}(
            chartest_cast<T, Common>{}(val),
            chartest_cast<Ch, Common>{}(ch));
    }

    constexpr int foo() const {
        return 0;
    }
  private:
    Ch ch;
};

template <template <class...> class Predicate = std::equal_to>
constexpr inline auto make_chartest_single = [](auto const ch) {
    return chartest_single<std::remove_cvref_t<decltype(ch)>, Predicate>{ch};
};

template <class... Tester>
struct chartest_conjunction {
    constexpr chartest_conjunction(Tester const&... tester)
    : testers{tester...}
    {}

    template <std::integral T>
    constexpr bool operator()(T const val) const noexcept {
        return std::apply([val](char_tester<T> auto const&... t) {
            return (t(val) && ...);
        }, testers);
    }

private:
    std::tuple<Tester const&...> testers;
};

template <class... Tester>
struct chartest_disjunction {
    constexpr chartest_disjunction(Tester const&... tester)
    : testers{tester...}
    {}

    template <std::integral T>
    constexpr bool operator()(T const val) const noexcept {
        return std::apply([val](char_tester<T> auto const&... t) {
            return (t(val) || ...);
        }, testers);
    }

private:
    std::tuple<Tester const&...> testers;
};

//constexpr inline chartest_single eol_cr{'\x0d'};
constexpr inline auto eol_cr = make_chartest_single<std::equal_to>(u8'\x0d');
constexpr inline auto eol_lf = make_chartest_single<std::equal_to>(u8'\x0a');

template <class... Tester>
struct chartest_multiple {
    using state_t = copy_tparam_t<std::variant, typename detail::ChartestMultiImpl<Tester...>::tail_t>;
    constexpr chartest_multiple(Tester const&... tester)
    : impl{tester...}
    {}

    constexpr state_t initialState() const noexcept {
        return {impl};
    }

    template <class T>
    constexpr bool operator()(std::tuple<T, state_t>& val) const noexcept {
        return std::visit([&val, this](auto const& f) {
            auto ret = f(get<0>(val));
            auto next = f.next();
            auto& stat = get<1>(val);
            if (next) {
                stat = ret ? *next : initialState();
                return false;
            } else {
                stat = initialState();
                return ret;
            }
        }, get<1>(val));
    }

    template <class T>
    constexpr std::tuple<bool, state_t> operator()(T const v, state_t const& state) const noexcept {
        return std::visit([v, this](auto& s) -> std::tuple<bool, state_t> {
            auto ret= s(v);
            auto next = s.next();
            if (next) {
                return {false, ret ? state_t{*next} : initialState()};
            } else {
                return {ret, initialState()};
            }
        }, state);
    }

private:
    detail::ChartestMultiImpl<Tester...> impl;
};

constexpr inline chartest_multiple eol_crlf{eol_cr, eol_lf};

} // namespace unebi