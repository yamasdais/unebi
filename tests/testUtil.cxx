#include <concepts>
#include <tuple>
#include <variant>
#include <functional>
#include <gtest/gtest.h>

#include <unebi/util.hpp>

#include "tools.hpp"

namespace unb = unebi;

struct TestSample {};
constexpr inline TestSample testSample;
constexpr auto foostr = "foo";

constexpr std::tuple sample0{12, foostr, testSample};
using sample0_t = decltype(sample0);

TEST(Util, GetTemplateParamIdx) {

    static_assert(
        std::same_as<int, unb::get_tparam_t<sample0_t, 0>>, "get_tparam<0>");
    static_assert(
        std::same_as<char const*, unb::get_tparam_t<sample0_t, 1>>, "get_tparam<1>");
    static_assert(
        std::same_as<TestSample, unb::get_tparam_t<sample0_t, 2>>, "get_tparam<2>");
    if constexpr(std::same_as<int, unb::get_tparam_t<sample0_t, 2>>) {
        FAIL();
    }

    int v = 42;
    std::tuple<int&, int const&> vtup{v, v};
    static_assert(
        std::same_as<int&, unb::get_tparam_t<decltype(vtup), 0>>, "get_tparam, int&");
    static_assert(
        std::same_as<int const&, unb::get_tparam_t<decltype(vtup), 1>>, "get_tparam, int const&");

    SUCCEED();
}

TEST(Util, CopyTemplateParams) {
    using sampleVariant_t = unb::copy_tparam_t<std::variant, sample0_t>;
    static_assert(
        std::same_as<std::variant<int, char const*, TestSample>, sampleVariant_t>, "copy template parameters");

    SUCCEED();
}
