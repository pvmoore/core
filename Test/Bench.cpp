#include "stdafx.h"

using namespace core;

#pragma intrinsic(__rdtsc)
void benchStringIndexOf();
void benchMemset();
void benchCore();

void benchmark() {
#ifdef _DEBUG
	return;
#endif
	printf("\nPerforming benchmarks==============\n\n");	
	//benchMemset();
	//benchCore();
}

void benchMemset() {
	char dest[10000];
	memset(dest, 0, 10000);
	

	ulong start = __rdtsc();
	for(uint i=0; i<1000; i++) {
		memset(dest, '0', 1007); 
		//core_memset(dest, '0', 1007);
		//memcpy(dest, src, len);
		//core_memcpy(dest, src, len);
	}

	ulong end = __rdtsc();
	printf("%s\n", dest);
	printf("Memset bench took %u ticks\n", (uint)(end-start));
}

void benchCore() {
	ulong start=__rdtsc();
	{

	}
	ulong end = __rdtsc();
	printf("Core took %d ticks\n", (int)(end-start));
}