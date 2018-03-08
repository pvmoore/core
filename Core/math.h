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
};


} /// core