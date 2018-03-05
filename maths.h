#pragma once

namespace core {

class Math {
public:
	static const constexpr float PI = 3.141592653589793f;

	template<typename T>
	static constexpr T min(T a, T b) {
		return a<b ? a : b;
	}
	template<typename T>
	static constexpr T min(T a, T b, T c) {
		return a<b && a<c ? a :
			b<c ? b :
			c;
	}
	template<typename T>
	static constexpr T max(T a, T b) {
		return a>b ? a : b;
	}
	template<typename T>
	static constexpr T max(T a, T b, T c) {
		return a>b && a>c ? a :
			b>c ? b :
			c;
	}
	static constexpr float toRadians(float degs) {
		return degs * PI / 180.0f;
	}
	static constexpr float toDegrees(float rads) {
		return rads * 180.0f / PI;
	}
};


} // namespace core