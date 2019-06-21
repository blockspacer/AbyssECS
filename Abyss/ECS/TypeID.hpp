#pragma once

#include <cstddef>
#include <utility>

namespace Abyss {
	namespace ECS {
		class TypeID;
	}
}

class Abyss::ECS::TypeID final {
private:
    static inline std::size_t m_count{0};
    static inline std::size_t m_bitcount{0};

	using InfoType = std::pair<decltype(m_count), decltype(m_bitcount)>;

public:
	template <typename T>
	[[nodiscard]] static auto info() -> const InfoType &;
};

// std::is_base_of_v
template <typename T>
inline auto Abyss::ECS::TypeID::info() -> const InfoType &
{
	static decltype(info<T>()) info{m_count++, ((m_bitcount == 0) ? (m_bitcount = 1) : (m_bitcount = (m_bitcount + m_bitcount)))};
	return info;
}
