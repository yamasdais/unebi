#pragma once

#include <concepts>

#include <unebi/fwd/locator.hpp>
#include <unebi/util.hpp>

namespace unebi::detail {

template <locator Locator>
struct LocatorValueImpl {
    using type = get_tparam_t<Locator, 0u>;
};

}