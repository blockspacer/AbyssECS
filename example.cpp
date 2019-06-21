/*

	Abyss ECS Example
	6/21/2019 - by HakanaiRuby

*/

#include "Abyss/ECS.hpp"

#include <bitset>
#include <iostream>

class Bomberman final : protected Abyss::ECS::EntityAdmin {
public:
    explicit Bomberman()
    {
        auto entity{createEntity()};
		std::cout << hasComponent<std::string>(entity) << '\n';

        auto &str{addComponent<std::string>(entity, "Ts &&ts...")};
		std::cout << hasComponent<std::string>(entity) << '\n';
		str = "Hey";

		auto &wow{getComponent<std::string>(entity)};
		std::cout << wow << std::endl;

		deleteComponent<std::string>(entity);
		std::cout << hasComponent<std::string>(entity) << '\n';
    }

    ~Bomberman()
    {
    }

    inline auto MovementSystem() -> void
    {
        // Todo: Get all the entities with a position component
    }
};

auto main() -> int
{
    Bomberman game{};
    {
        auto &&[id, bitID]{Abyss::ECS::TypeID::info<std::string>()};
        std::cout << id << '\t' << std::bitset<sizeof(bitID) * 8>(bitID) << '\t' << bitID << '\n';
    }
    {
        auto &&[id, bitID]{Abyss::ECS::TypeID::info<std::size_t>()};
        std::cout << id << '\t' << std::bitset<sizeof(bitID) * 8>(bitID) << '\t' << bitID << '\n';
    }
    {
        auto &&[id, bitID]{Abyss::ECS::TypeID::info<std::int8_t>()};
        std::cout << id << '\t' << std::bitset<sizeof(bitID) * 8>(bitID) << '\t' << bitID << '\n';
    }
    {
        auto &&[id, bitID]{Abyss::ECS::TypeID::info<std::int64_t>()};
        std::cout << id << '\t' << std::bitset<sizeof(bitID) * 8>(bitID) << '\t' << bitID << '\n';
    }
    {
        auto &&[id, bitID]{Abyss::ECS::TypeID::info<std::uint32_t>()};
        std::cout << id << '\t' << std::bitset<sizeof(bitID) * 8>(bitID) << '\t' << bitID << '\n';
    }
    return 0;
}
