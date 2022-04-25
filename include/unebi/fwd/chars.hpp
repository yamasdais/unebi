#pragma once

#include <concepts>

namespace unebi {


template <class Tester, class T>
concept char_tester = std::predicate<Tester, std::add_const_t<T>>;

template <std::integral Ch, template <class...> class Predicate>
struct chartest_single;

template <class... Tester>
struct chartest_disjunction;

// customization point
template <std::integral From, std::integral To>
struct chartest_cast;

template <std::integral Sample, std::integral Op>
struct chartest_common_type;

} // namespace unebi