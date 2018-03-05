#include "stdafx.h"

#include <vector>
#include <chrono>

using namespace core;
using std::vector;
using std::chrono::high_resolution_clock;

void benchMemset();
void benchCore();
void benchCharBuf();

void benchmark() {
#ifdef _DEBUG
	printf("============== Not running benchmarks in DEBUG mode\n\n");
	return;
#endif
	printf("============== Running benchmarks\n\n");	

	benchCharBuf();
	//benchMemset();
	//benchCore();
}

void benchMemset() {
	char dest[10000];
	memset(dest, 0, 10000);
	
	auto start = high_resolution_clock::now();
	for(uint i=0; i<1000; i++) {
		memset(dest, '0', 1007); 
		//core_memset(dest, '0', 1007);
		//memcpy(dest, src, len);
		//core_memcpy(dest, src, len);
	}
	auto end = high_resolution_clock::now();
	auto elapsedMs = (end - start).count() * 1e-6;
	printf("%s\n", dest);
	printf("Elapsed: %.3f ms\n", elapsedMs);
}

void benchCore() {
	auto start = high_resolution_clock::now();
	{

	}
	auto end = high_resolution_clock::now();
	auto elapsedMs = (end - start).count() * 1e-6;
	printf("Elapsed: %.3f ms\n", elapsedMs);
}

void benchCharBuf() {
	auto start = high_resolution_clock::now();

	auto end = high_resolution_clock::now();
	auto elapsedMs = (end-start).count() * 1e-6;
	printf("Elapsed: %.3f ms\n", elapsedMs);
}