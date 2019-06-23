/*

	Abyss ECS Example
	6/21/2019 - by HakanaiRuby

*/

#include "Abyss/ECS.hpp"

#include <chrono>
#include <iostream>

struct Position final {
    float x{0}, y{0}, dx{1}, dy{1};
};

struct Position2 final {
    float x{0}, y{0}, dx{1}, dy{1};
};

struct Renderer {};

class Bomberman final : protected Abyss::ECS::EntityAdmin {
public:
    explicit Bomberman()
    {
        for (std::size_t i{0}; i < 4096; ++i) {
            auto ent{createEntity()};
            addComponent<Position>(ent);
            addComponent<Position2>(ent);

            if (i == 0)
                addComponent<Renderer>(ent);
        }
    }

    ~Bomberman()
    {
    }

    inline auto MovementSystem(float dt) -> void
    {
        auto entities{getEntitiesWithComponents<Position>()};

        for (auto &&[entity, position] : entities) {
            position.x += position.dx * dt;
            position.y += position.dy * dt;
        }
    }

    inline auto Movement2System(float dt) -> void
    {
        auto entities{getEntitiesWithComponents<Position2>()};

        for (auto &&[entity, position] : entities) {
            position.x += position.dx * dt;
            position.y += position.dy * dt;
        }
    }

    inline auto RendererSystem(float) -> void
    {
        auto entities{getEntitiesWithComponents<Position, Renderer>()};

        for (auto &&[entity, position, renderer] : entities)
            std::cout << position.x << ' ' << position.y << '\n';
        std::cout << '\n';
    }

    inline auto think(float dt)
    {
        MovementSystem(dt);
        Movement2System(dt);
    }

    inline auto paint(float dt)
    {
        RendererSystem(dt);
    }
};

auto main() -> int
{
    Bomberman                                          game{};
    std::chrono::time_point<std::chrono::system_clock> now{std::chrono::system_clock::now()};

    while (true) {
        auto dt{static_cast<float>(std::chrono::nanoseconds(std::chrono::system_clock::now() - now).count()) / 1000000000.0f};
        now = std::chrono::system_clock::now();
        game.think(dt);
        game.paint(dt);
    }
    return 0;
}
