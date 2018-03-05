#include "stdafx.h"

using namespace core;

void testFile();
void testThreads();
void testArray();
void testCharBuf();
void testHashMap();
void testLinkedList();
int someMethod(int a, int b, int c, int d);

void testString();
void benchmark();

#pragma intrinsic(__rdtsc)

int _tmain(int argc, _TCHAR* argv[]) {
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetAllocHook(MyAllocHook);
	printf("Test running in DEBUG mode\n");
#endif
#ifdef NDEBUG 
	printf("Test running in RELEASE mode\n");
#endif

	printf("Testing core namespace\n");

	printf("size_t=%zu\n", sizeof(size_t));
	printf("ptrdiff_t=%zu\n", sizeof(ptrdiff_t));
	printf("DWORD=%zu\n", sizeof(DWORD));

	testFile();
	testString();
	testThreads();
	testCharBuf();
	testLinkedList();

	//someMethod(1,2,3,4);

	benchmark();

	printf("\nFinished. Press ENTER");
	getchar();

	return 0;
}

int someMethod(int a, int b, int c, int d) {
	printf("%d%d%d%d\n",a,b,c,d);
	return a+b+c+d;
}

void assertEquals(const char* a, const char* b) {
	if(a==b) return;
	if(a==NULL || b==NULL) assert(0);
	if(strlen(a) != strlen(b)) assert(0);
	int i = 0;
	while(a[i] && b[i]) {
		if(a[i] != b[i]) assert(0);
		i++;
	}
}	

uint _parseUint(char* buf, uint n) {
	uint pos = 30;
	do{
		uint rem = (uint)(n%10);
		n /= 10;
		buf[--pos] = '0'+rem;
	}while(n>0);
	return pos;
}

void testString() {
	printf("\n\nTesting String========================\n");
	
	// constructors
	String s1;
	String s2(true);
	String s3(12);
	String s4("hello");
	String s5('l');		
	String s6(0x7fffffffffffffffULL);
	String s7(3.7f);
	String s8(4.4);
	String s9(s2);
	String s10(String("poop"));

	assert(s1=="");
	assert(s2=="true");
	assert(s3=="12");
	assert(s4=="hello");
	assert(s5=="l");
	assert(s6=="9223372036854775807");
	assert(s7=="3.7");
	assert(s8=="4.4");
	assert(s9=="true");
	assert(s10=="poop");

	String a1 = "peter moore";
	String a2("monkey magic");
	String a3("peter moore woz here", 0, 15);
	String a4("peter moore", 1, 5);
	String a5(a3, 0, 11);
	String a6(a3, 6, 11);

	assert(a1=="peter moore");
	assert(a2=="monkey magic");
	assert(a3=="peter moore woz");
	assert(a4=="eter");
	assert(a5=="peter moore");
	assert(a6=="moore");
	
	//printf("s1 = %s\n", s1.cStr());
	//printf("s2 = %s\n", s2);
	//printf("s3 = %s\n", s3);
	//printf("s4 = %s\n", s4);
	//printf("s5 = %s\n", s5);
	//printf("s6 = %s\n", s6);
	//printf("s7 = %s\n", s7);
	//printf("s8 = %s\n", s8);
	//printf("s9 = %s\n", s9);
	//printf("s4[2] = %c\n", s4[2]);
	
	// copy constructors
	String s11 = s2;		// copy construct
	String s12 = "there";	// copy construct
	assert(s11=="true");
	assert(s12=="there");
	//printf("s10 = %s\n", s10);
	//printf("s11 = %s\n", s11);

	// copy assignment
	//printf("--- copy assignment ---\n");
	String s13("original");
	s13 = s4;
	assert(s13=="hello");
	//printf("%s\n", s12);
	String s13_2 = "green";
	assert(s13_2=="green");
	//printf("%s\n", s12_2);


	printf("--- indexOf ---\n");
	assert(s4.indexOf("h")==0 && s4.indexOf("e")==1 && s4.indexOf("l")==2 && s4.indexOf("o")==4 && s4.indexOf("w")==-1);
	assert(s4.indexOf(s5)==2 && s4.indexOf(s2)==-1);

	/*
	//printf("--- lastIndexOf ---\n");
	assert(s4.lastIndexOf("h")==0 && s4.lastIndexOf("e")==1 && s4.lastIndexOf("l")==3 && s4.lastIndexOf("o")==4 && s4.lastIndexOf("w")==-1);

	printf("--- substring ---\n");
	String ss1 = "Peter moore";
	String ss2 = ss1.substring(0, 11);
	//String ss3 = ss1.substring(6);
	//assert(ss2=="Peter moore");
	//assert(ss3=="moore");

	//printf("--- cStr ---\n");
	//String c1 = "peter";
	//assert(core_strcmp(c1.cStr(), "peter", 5));

	//printf("getUpperCase\n");
	//String s13("Hello there");
	//printf("%s --> %s\n", s13, s13.getUpperCase());
	*/

	printf("--- lowerCased() ---\n");
	String lc1("This is a string. Hi there!");
	StringRef lc2 = lc1.lowerCased();

	// TODO - this is crap
	assert(*(lc2.get())=="this is a string. hi there!");
}

DWORD doSomething(void* args) {
	uint* p = (uint*)args;
	uint arg = p[0];
	printf("[%u] I am the new thread. Arg is %u...\n", 
		Thread::currentThreadId(), arg);
	printf("[%u] Thread finishing\n", Thread::currentThreadId());
	return 7;
}
void testThreads() {
	printf("\n\nTesting threads.h\n==================\n");

	printf("[Main] thread ID is %u\n", Thread::currentThreadId());
	uint arg = 1;

	auto t1 = new Thread(doSomething, &arg);
	auto t2 = new Thread(doSomething, &arg);
	t1->start();
	t2->start();
	printf("t1 id = %u\n", t1->id);
	printf("t1 status before join = %u\n", t1->getStatus());
	printf("t1 exit code before join = %u\n", t1->getExitCode());
	t1->sleep(100);
	t2->yield();
	
	t1->join();
	t2->join();
	printf("[Main] I am the main thread again\n");
	printf("t1 status = %u\n", t1->getStatus());
	printf("t2 status = %u\n", t2->getStatus());
	printf("t1 exit code = %u\n", t1->getExitCode());
	printf("t2 exit code = %u\n", t2->getExitCode());

	printf("t1 cycles used = %zu\n", t1->getCyclesUsed());

	delete t1;
	delete t2;
}

void testCharBuf() {
	printf("\n\nTesting CharBuf\n==================\n");
	CharBuf buf("hello");
	CharBuf* buf2 = new CharBuf("there");
	CharBuf buf3;
	CharBuf buf4(buf);
	CharBuf buf5;
	CharBuf buf6("ABCD", 2);

	buf.append(" ");
	buf.append(buf2->cStr());
	buf5.append("abcd", 3);

	printf("%s (%d)\n", buf.cStr(), buf.length());
	printf("%s (%d)\n", buf2->cStr(), buf2->length());
	printf("%s (%d)\n", buf3.cStr(), buf3.length());
	printf("%s (%d)\n", buf4.cStr(), buf4.length());
	printf("%s (%d)\n", buf5.cStr(), buf5.length());
	printf("%s (%d)\n", buf6.cStr(), buf6.length());

	// test append string and chaining
	CharBuf a1("a");
	a1.append("b").append("c");
	a1 += "d";
	printf("%s (%d)\n", a1.cStr(), a1.length());

	CharBuf i1("abcdef");

	// test indexOf(char,int,int)
	assert(i1.indexOf('a')==0);
	assert(i1.indexOf('f')==5);
	assert(i1.indexOf('z')==-1);

	assert(i1.indexOf('c', 0)==2);
	assert(i1.indexOf('c', 1)==1);
	assert(i1.indexOf('c', 2)==0);
	assert(i1.indexOf('c', 3)==-1);

	assert(i1.indexOf('c', 0, 6)==2);
	assert(i1.indexOf('f', 0, 6)==5);
	assert(i1.indexOf('f', 0, 5)==-1);
	assert(i1.indexOf('f', 1, 6)==4);
	assert(i1.indexOf('f', 5, 6)==0);

	// test indexOf(char*,int,int)
	assert(i1.indexOf("a")==0);
	assert(i1.indexOf("f")==5);
	assert(i1.indexOf("z")==-1);
	assert(i1.indexOf("bc")==1);
	assert(i1.indexOf("ba")==-1);
	assert(i1.indexOf("abcdef")==0);
	assert(i1.indexOf("ef")==4);

	assert(i1.indexOf("ef", 0)==4);
	assert(i1.indexOf("ef", 1)==3);
	assert(i1.indexOf("ef", 4)==0);
	assert(i1.indexOf("ef", 5)==-1);
	assert(i1.indexOf("abcdef", 1)==-1);
	
	assert(i1.indexOf("ef", 0, 6)==4);
	assert(i1.indexOf("ef", 0, 5)==-1);
	assert(i1.indexOf("ef", 1, 6)==3);
	assert(i1.indexOf("ef", 4, 6)==0);

	// test append int, double and clear
	CharBuf b1;
	b1.append("one").append(1);
	assertEquals("one1", b1.cStr());
	b1.append("two").append(2);
	assertEquals("one1two2", b1.cStr());
	b1.clear();
	b1.append(-1);
	assertEquals("-1", b1.cStr());
	b1.append(123456789);
	assertEquals("-1123456789", b1.cStr());
	b1.clear();
	b1.append(3.14, 2);
	assertEquals("3.14", b1.cStr());

	// test operator[]
	b1.clear();
	b1 += "abcdefg";
	assert(b1[0]=='a');
	assert(b1[6]=='g');
	b1[1] = 'B';
	assertEquals("aBcdefg", b1.cStr());

	delete buf2;
}

void testLinkedList() {
	printf("\n\nTesting LinkedList\n==================\n");
	
	LinkedList<uint> list;

	list.add(2);
	list.add(10);
	list.add(100);
	list.add(200);
	list.add(300);
	list.add(400);

	printf("\n");
	printf("\n%s", list.toDebugString());

	list.insert(5, 1);
	printf("\n");
	printf("\n%s", list.toDebugString());

	list.insert(1, 0);
	printf("\n");
	printf("\n%s", list.toDebugString());

	list.insert(500, 8);
	printf("\n");
	printf("\n%s", list.toDebugString());


	uint i1 = list.remove(0);
	printf("\n\n");
	printf("\n%s", list.toDebugString());

	uint i2 = list.remove(1);
	printf("\n");
	printf("\n%s", list.toDebugString());

	uint i3 = list.remove(6);
	printf("\n");
	printf("\n%s", list.toDebugString());

	list[0] = 33;

	printf("\n%s\n", list.toString());

	printf("\nRandom list test");
	// some random testing

	int R[] = {
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0,
		8,8,5,5,0,3,2,7,5,0,1,6,1,5,6,7,1,6,8,7,2,8,9,9,8,9,1,4,8,2,7,2,3,3,2,5,4,2,4,6,3,3,0,3,4,9,7,6,0,2,1,5,9,0,4,4,6,2,4,5,0,2,7,8,4,7,3,2,0,8,3,3,
		4,3,1,0,4,2,2,6,6,5,0,3,1,7,1,4,3,4,4,5,0,5,6,3,7,5,0,3,6,2,3,5,6,7,7,1,6,9,3,4,8,3,4,5,7,0,0,2,6,9,0,0,6,5,2,1,1,8,8,6,3,3,6,5,8,4,2,5,4,8,1,6,3,5,6,8,9,7,2,1,
		9,0,7,6,5,0,3,0,0,2,1,8,0,9,8,1,6,3,1,2,4,6,1,9,5,1,5,6,3,1,6,4,8,4,1,1,2,1,3,4,5,7,4,5,1,8,9,0
	};

	srand(0);
	LinkedList<uint> ll;
	ulong start = __rdtsc();
	for(int i=0; i<1600; i++) {
		int r	 = R[i];
		uint val = R[i];
		switch(r) {
			case 0:  
			case 1:
			case 2:
			case 3:
			case 4:
			case 5: ll.add(val); break;
			case 6:	if(!ll.isEmpty()) ll.insert(val, (r*77)%ll.size()); break;
			case 7:	if(!ll.isEmpty()) ll.remove((r*77)%ll.size()); break;
			case 8:	if(!ll.isEmpty()) ll.removeHead(); break;
			case 9: if(!ll.isEmpty()) ll.removeTail(); break;
			default: assert(0);
		}
	}
	ulong end = __rdtsc();
	
	printf("\n%s", ll.toString());
	//printf("\n\n%s", ll.toDebugString());
	printf("\nTook %zu ticks", end-start);
}

void testFile() {
	printf("\n\nTesting File\n==================\n");	
}

