/// Project statics.
/// This file is also configured to create the pre-compiled header

#include "_pch.h"
#include "core.h"

namespace core {

FILE* Log::fp = nullptr;
std::wstring Log::filename;

Log singletonLog;

} /// core