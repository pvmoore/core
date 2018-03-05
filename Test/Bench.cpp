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
	benchStringIndexOf();	
	//benchMemset();
	//benchCore();
}

void benchStringIndexOf() {
	String s1("abcdefghij0123456789klmnopqrstuvwABCDEFGHIJKLMNOPQRSTUVWXYZxyz");
	String s2("xyz");
	const char* xyz = "xyz";
	const int ITERATIONS = 100000;
	volatile int n = -2;

	ulong start = __rdtsc();
	for(uint i=0; i<ITERATIONS; i++) {
		n = s1.indexOf(s2);
		//n = s1.indexOf(xyz);
		//volatile String s = s1.substring(0, 5);
	}
	ulong end = __rdtsc();
	printf("Took %u ticks (%g per iteration) %d\n", (uint)(end-start), (double)(end-start)/(double)ITERATIONS, n);
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
	//printf("%d\n", core_div_10(123));
	volatile int a = core_div_10(100);
	ulong end = __rdtsc();
	printf("Core took %d ticks\n", (int)(end-start));
}