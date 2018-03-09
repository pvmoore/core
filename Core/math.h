#pragma once

namespace core {

class Math {
public:
	static const constexpr float PI = 3.141592653589793f;

	static constexpr float toRadians(float degs) {
		return degs * PI / 180.0f;
	}
	static constexpr float toDegrees(float rads) {
		return rads * 180.0f / PI;
	}
	static std::vector<uint> factorsOf(uint n) {
		/// this is not the best way of doing this
		std::vector<uint> factors;
		factors.reserve(n / 2);
		uint i = n;
		while(i>0) {
			if((n%i) == 0) factors.push_back(i);
			i--;
		}
		return factors;
	}
};


} /// core