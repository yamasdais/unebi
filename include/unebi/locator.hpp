#pragma once

#include <concepts>

#include <unebi/util.hpp>
#include <unebi/fwd/locator.hpp>

#include <unebi/detail/locator.hpp>

namespace unebi {

template <class Locator>
using locator_value_t = typename detail::LocatorValueImpl<Locator>::type;

//
struct index_locator {
    constexpr explicit index_locator(size_t index = 0u) noexcept
    : index{index}
    {}
    constexpr index_locator(index_locator const&) noexcept = default;
    constexpr index_locator(index_locator&&) noexcept = default;
    constexpr index_locator& operator=(index_locator const&) noexcept = default;
    constexpr index_locator& operator=(index_locator&&) noexcept = default;

    constexpr void increment() noexcept {
        ++index;
    }
    constexpr index_locator next() const noexcept {
        return index_locator{index + 1u};
    }
    constexpr auto position() const noexcept {
        return index;
    }

  private:
    size_t index;
};

// Specialized 'locator_value_t<Locator>'
template <>
struct detail::LocatorValueImpl<index_locator> {
    using type = void;
};

//
template <class T>
struct text_locator {

    private:
    size_t column;
    size_t row;
};

}