#pragma once

namespace core {

template<typename COLLECTION, typename LAMBDA>
void forEach(COLLECTION collection, LAMBDA lambda) {
	std::for_each(collection.begin(), collection.end(), lambda);
}

/// Transforms elements of same type within the collection.
template <typename COLLECTION, typename LAMBDA>
COLLECTION map(COLLECTION collection, LAMBDA lambda) {
	std::transform(collection.begin(), collection.end(), collection.begin(), lambda);
	return collection;
}

}