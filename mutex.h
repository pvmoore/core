#pragma once

namespace core {

class Mutex {
	HANDLE handle = nullptr;
public:
	Mutex() : handle(CreateMutex(nullptr, FALSE, nullptr)) {}
	~Mutex() {
		if(handle) {
			CloseHandle(handle);
			handle = nullptr;
		}
	}
	/// Lock the mutex. Returns true if the mutex was successfully locked.
	bool lock(uint millis=INFINITE) const {
		uint r = WaitForSingleObject(handle, millis);
		return r == WAIT_OBJECT_0;
	}
	void unlock() const {
		assert(ReleaseMutex(handle));
	}
};

} // core
