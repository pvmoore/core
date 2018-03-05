#pragma once
///
///	Fast simple queue. CAPACITY must be a power of 2.
///

#include <intrin.h>

namespace core {

template<typename T, uint CAPACITY>
class Queue {
	T array[CAPACITY];
	uint r = 0, w = 0, mask = CAPACITY - 1;
	static_assert(__popcnt(CAPACITY) == 1);
public:
	Queue() { static_assert(__popcnt(CAPACITY) == 1); }
	uint length() const { return w - r; }
	bool empty() const { return length() == 0; }
	auto push(T value) {
		uint p = w++;
		array[p&mask] = value;
		return *this;
	}
	T pop() {
		uint p = r++;
		return array[p&mask];
	}
};

} /// core