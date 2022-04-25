#pragma once

#include <concepts>

namespace unebi {

template <class Locator>
concept locator =
    std::semiregular<std::remove_cvref_t<Locator>>
    && requires (Locator l) {
    std::as_const(l).position();
};

namespace detail {

template <locator Locator>
struct LocatorValueImpl;

}

template <class Locator>
concept locator_without_value =
    std::same_as<void, typename detail::LocatorValueImpl<Locator>::type>;

template <class Locator>
concept incrementable_locator_without_value =
    locator<Locator>
    && requires(Locator& locator) {
        locator.increment();
        {locator.next()} -> std::same_as<Locator>;
    };

template <class Locator, class T>
concept incrementable_locator_with_value =
    locator<Locator>
    && requires(Locator& locator, T const v) {
        locator.increment(v);
        {locator.next(v)} -> std::same_as<Locator>;
    };

template <class Locator>
concept incrementable_locator =
    (locator_without_value<Locator> && incrementable_locator_without_value<Locator>)
    || incrementable_locator_with_value<Locator, detail::LocatorValueImpl<Locator>>
    ;

// forward declarations
struct index_locator;
}