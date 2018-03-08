#pragma once

#include <Windows.h>

namespace core {

class Semaphore {
	HANDLE handle;
public:
	Semaphore(int initialCount, int maxCount=INFINITE) : 
		handle(CreateSemaphore(nullptr, initialCount, maxCount, nullptr)) {}
	~Semaphore() {
		if(handle) {
			CloseHandle(handle);
			handle = nullptr;
		}
	}
	/*
	 * Reduce the semaphore count by 1 and return. If the count
	 * is 0 then wait for it to be notified or timeout.
	 * Return true if we have been signalled.
	 */
	bool wait(uint millis=INFINITE) const {
		auto r = WaitForSingleObject(handle, millis);
		return r == WAIT_OBJECT_0;
	}
	/* Increase the semaphore count by 1, signalling any
	 * waiting threads.
	 */
	void notify() const {
		ReleaseSemaphore(handle, 1, nullptr);
	}
};

} /// core