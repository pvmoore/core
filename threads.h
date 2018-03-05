#pragma once

#include <atomic>

namespace core {

class Thread;

class ThreadPool {
public:
	ThreadPool() {}
	~ThreadPool() {}
};

DWORD thread_wrapper_func(void* argPtr);

class Thread {
public:
	enum Status : uint { NOT_STARTED, RUNNING, FINISHED };
	struct FuncWrapperArgs {
		Thread* t;
		DWORD(*func)(void*);
		void* args;
	};
	DWORD id; // thread 
private:
	HANDLE handle;
	std::atomic<uint> status;
	FuncWrapperArgs args;

	

	

	void close() {
		if(handle) {
			// this might throw an exception if _endthread has been called
			CloseHandle(handle);
			handle = nullptr;
		}	
		setStatus(FINISHED);
	}
public:
	Status getStatus() { return (Status)status.load(); }
	void setStatus(Status s) { status.store(s); }

	Thread(DWORD(*function)(void*), void* functionArgs) :
		handle(nullptr),
		args({this,function,functionArgs}),
		status(NOT_STARTED)
	{
	}
	~Thread() {
		close();
	}

	void start() {
		if(getStatus() == NOT_STARTED) {
			handle = CreateThread(nullptr, 0, thread_wrapper_func, &args, 0, &id);
			setStatus(RUNNING);
		}
	}
	/*
	 * THREAD_PRIORITY_HIGHEST		= 2
	 * THREAD_PRIORITY_ABOVE_NORMAL = 1
	 * THREAD_PRIORITY_NORMAL		= 0
	 * THREAD_PRIORITY_BELOW_NORMAL = -1
	 * THREAD_PRIORITY_LOWEST		= -2
	 */
	int getPriority() {
		return GetThreadPriority(handle);
	}
	void setPriority(int p) {
		BOOL result = SetThreadPriority(handle, p);
	}
	bool isFinished() { 
		return getStatus() == FINISHED;
	}
	DWORD getExitCode() {
		if(getStatus() != FINISHED) return STILL_ACTIVE;

		DWORD exitCode;
		GetExitCodeThread(handle, &exitCode);
		return exitCode;
	}
	void join(DWORD millis=INFINITE) {
		if(getStatus() == RUNNING) {
			auto result = WaitForSingleObject(handle, millis);
		}
	}
	void suspend() {
		auto result = SuspendThread(handle);
	}
	void resume() {
		auto result = ResumeThread(handle);
	}
	ulong getCyclesUsed() {
		ulong ticks;
		QueryThreadCycleTime(handle, &ticks);
		return ticks;
	}

	//================================================  statics

	// Park current thread for millis ms. Switches to another thread
	static void sleep(uint millis) {
		Sleep(millis);
	}
	// Allow another thread on the same processor to run
	// if available, otherwise return immediately.
	static void yield() {
		auto success = SwitchToThread();
	}
	static DWORD currentThreadId() {
		return GetCurrentThreadId();
	}
};

} // namespace core