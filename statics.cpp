#include "stdafx.h"

namespace core {

// threads.h
DWORD thread_wrapper_func(void* argsPtr) {
	auto args = *(Thread::FuncWrapperArgs*)argsPtr;

	// call the user function
	DWORD exitCode = args.func(args.args);

	// this thread is now exiting
	args.t->setStatus(Thread::FINISHED);

	return exitCode;
}

} // namespace core