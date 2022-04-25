#include <gtest/gtest.h>

#include <unebi/util.hpp>

#include "tools.hpp"

namespace unb = unebi;

TEST(IndexLocator, Concepts) {
    using locator_t = unb::index_locator;
    static_assert(std::is_nothrow_default_constructible_v<locator_t>, "default constructible");
    static_assert(std::semiregular<locator_t>, "semiregular");
    static_assert(unb::locator<locator_t>, "locator concept");
    static_assert(std::same_as<void, unb::locator_value_t<locator_t>>, "locator_value_t");
    static_assert(unb::locator_without_value<locator_t>, "locator_without_value");
    static_assert(unb::incrementable_locator_without_value<locator_t>, "incrementable_locator_without_value");
    static_assert(unb::incrementable_locator<locator_t>, "incrementable_locator");

    SUCCEED();
}

TEST(IndexLocator, Increment) {
    constexpr unb::index_locator loc;
    ASSERT_EQ(0, loc.position());
    constexpr auto loc1 = loc.next();
    ASSERT_EQ(1, loc1.position());

}