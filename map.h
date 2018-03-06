#pragma once

namespace core {

class Map {
public:
	template<typename K, typename V>
	bool contains(std::unordered_map<K, V>& map, V value) {
		return map.find(value) != map.end();
	}
};

} /// core