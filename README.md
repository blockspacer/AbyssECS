# Abyss ECS
Abyss ECS is an experimental and standalone, fast C++17 entity component system library.  

## Requirements
- A C++17 compatible compiler (clang++ is strongly recommended)
- Basic yet modern C++ knowledge

## Installation
Abyss ECS is a header only library, you just need to include ``Abyss/ECS.hpp`` to start using the ECS.

## Usage
To use Abyss ECS, you will need to instantiate an object of type ``Abyss::ECS::EntityAdmin``.  
The ``EntityAdmin`` enforces the following rules:
- Ownership over the components meaning they are destroyed once the EntityAdmin.
- A custom memory manager in order to yoink as much performance as we can by using custom destructors.
- Usage of placement new for maximum performance by recycling previously created component of deceased entities.
- Features a pool for each component type which is indexed for O(1) access time.
- The pools are stored next to each others which is designed to be cache efficient.

```cpp
#include "Abyss/ECS.hpp"

#include <iostream>
#include <string>

auto main() -> int
{
	Abyss::ECS::EntityAdmin admin{};

	auto &entity{admin.createEntity()};

	admin.addComponent<std::string>(entity, "Deep down in the Abyss...");
	
	std::cout << admin.getComponent<std::string>(entity) << '\n';

	admin.destroyEntity();
	return 0;
}
```
Details: 
The ``EntityAdmin`` has the ownership over the components created, so when the ``EntityAdmin`` is deleted/goes out of scope, the components stored will be destructed.  
### Note: The class isn't marked ``final`` and therefore, it is possible to derivate your world from the EntityAdmin.
```cpp
class World final : private Abyss::ECS::EntityAdmin {
public:
	explicit World(const std::wstring &mapFile)
	{
		// Example: Parse and load mapFile
		// Example: Create the world's default entities
	}
}
```

## Example
```cpp
// Todo
```

## Notes
- This library is mainly tested using clang++, although other compilers should work fine.  
- In case an issue happens, feel free to open an issue containing a test project where an issue occurs.
- Once the C++20 modules will be released, Abyss ECS will be converted to support this feature.

## Warning
The current state of this library is very experimental, the API might change at any time!
