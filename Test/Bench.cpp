#include "_pch.h"
#include "../core.h"

using namespace core;
using std::vector;
using std::chrono::high_resolution_clock;
using std::string;

void benchMemset();
void benchCore();
void benchCharBuf();

void benchmark() {
	printf("Benchmarking...\n\n");	

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
	printf("Benching CharBuf\n");
	CharBuffer buf;
	string str;
#define CB
	auto start = high_resolution_clock::now();
	for(int j = 0; j < 1000; j++) {
#ifdef CB
		buf.clear();
#else
		str.clear();
		str.shrink_to_fit();
#endif
		for(int i = 0; i < 100000; i++) {
#ifdef CB
			buf.append("hello");
#else
			str += "hello";
#endif
		}
	}
	auto end = high_resolution_clock::now();
	printf("CB  Result = %u\n", buf.length());
	printf("STR Result = %llu\n", str.size());
	auto elapsedMs = (end-start).count() * 1e-6;
	printf("Elapsed: %.3f ms\n", elapsedMs);
}