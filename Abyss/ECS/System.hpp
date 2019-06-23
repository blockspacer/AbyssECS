#pragma once

#include "TypeID.hpp"

namespace Abyss {
    namespace ECS {
        class ISystem;

        template <typename... Ts>
        class System;
    }
}

class Abyss::ECS::ISystem {
public:
    virtual ~ISystem() = default;

    virtual auto update(double dt) -> void = 0;
};

template <typename... Ts>
class Abyss::ECS::System {
};
