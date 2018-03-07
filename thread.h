#pragma once
///
///	Todo - manage thread creation etc from within a ThreadPool.
///

#include <atomic>
#include <Windows.h>

namespace core {

class Thread;

class ThreadPool {
public:
	ThreadPool() {}
	~ThreadPool() {}
};

class Thread {
public:
	enum Status : uint { NOT_STARTED, RUNNING, FINISHED };
	struct FuncWrapperArgs {
		Thread* t;
		DWORD(*func)(void*);
		void* args;
	};
private:
	HANDLE handle = nullptr;
	std::atomic<uint> status = Status::NOT_STARTED;
	FuncWrapperArgs args;

	void close() {
		if(handle) {
			// this might throw an exception if _endthread has been called
			CloseHandle(handle);
			handle = nullptr;
		}
		setStatus(Status::FINISHED);
	}
public:
	DWORD id; 
	std::string name;

	Thread(std::string name, DWORD(*function)(void*), void* functionArgs) : 
		name(name), args({this,function,functionArgs}) {}
	~Thread() {
		close();
	}

	Status getStatus() { return (Status)status.load(); }
	void setStatus(Status s) { status.store(s); }
	void start() {
		if(getStatus() == Status::NOT_STARTED) {

			const auto wrapper = [](void* argsPtr)->DWORD {
				auto args = *(Thread::FuncWrapperArgs*)argsPtr;

				// call the user function
				uint exitCode = args.func(args.args);

				// this thread is now exiting
				args.t->setStatus(Thread::FINISHED);

				return exitCode;
			};

			handle = CreateThread(nullptr, 0, wrapper, &args, 0, &id);
			setStatus(Status::RUNNING);
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
		return getStatus() == Status::FINISHED;
	}
	DWORD getExitCode() {
		if(getStatus() != Status::FINISHED) return STILL_ACTIVE;

		DWORD exitCode;
		GetExitCodeThread(handle, &exitCode);
		return exitCode;
	}
	void join(DWORD millis=INFINITE) {
		if(getStatus() == Status::RUNNING) {
			auto result = WaitForSingleObject(handle, millis);
		}
	}
	/// Suspends the thread
	void suspend() {
		auto result = SuspendThread(handle);
	}
	/// Resumes from suspension
	void resume() {
		auto result = ResumeThread(handle);
	}
	ulong getCyclesUsed() const {
		ulong ticks;
		QueryThreadCycleTime(handle, &ticks);
		return ticks;
	}

	//================================================  statics

	/// Park current thread for millis ms. Switches to another thread
	static void sleep(uint millis) {
		Sleep(millis);
	}
	/// Allow another thread on the same processor to run
	/// if available, otherwise return immediately.
	static void yield() {
		SwitchToThread();
	}
	static uint currentThreadId() {
		return GetCurrentThreadId();
	}
};

} /// core