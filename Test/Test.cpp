#include "_pch.h"
#include "../Core/core.h"

using namespace core;
using std::string;
using std::vector;

void benchmark();

int wmain(int argc, const wchar_t* argv[]) {
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif
#ifdef _DEBUG
	printf("Testing...\n\n");

#else
	benchmark();
#endif
	printf("\nFinished. Press ENTER");
	getchar();
	return 0;
}



