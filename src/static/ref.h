#pragma once

#include <memory>

namespace MicroNinja::Ref {

    template<typename T>
    using Unique = std::unique_ptr<T>;

    template<typename T>
    const auto make = std::make_unique<T>;
    
}