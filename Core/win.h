#pragma once

#include <Windows.h>

namespace core {

class Win final {
public:
	static std::string HRESULTToString(HRESULT hr) {
		char buf[256];
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
					   nullptr, hr,
					   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), /// Default language
					   buf,
					   256,
					   nullptr);
		return buf;
	}
};

} /// core