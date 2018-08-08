#pragma once

#include <unordered_map>

namespace core {

class Map final {
public:
	template<typename K, typename V>
	bool contains(std::unordered_map<K, V>& map, V value) {
		return map.find(value) != map.end();
	}
};

} /// core