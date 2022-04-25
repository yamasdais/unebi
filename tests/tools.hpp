#pragma once

#include <concepts>
#include <gtest/gtest.h>

#include <unebi/locator.hpp>

namespace testutil {

using TestCharTypes = ::testing::Types<char, char8_t, char16_t, char32_t, wchar_t>;

}