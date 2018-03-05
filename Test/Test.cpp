#include "stdafx.h"

using namespace core;
using std::string;
using std::vector;

void testFile();
void testThreads();
void testCharBuf();
int someMethod(int a, int b, int c, int d);

void testString();
void benchmark();

int wmain(int argc, const wchar_t* argv[]) {
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetAllocHook(MyAllocHook);
#endif
#ifdef NDEBUG 
	static_assert(false, "Run these tests in DEBUG");
#endif
	printf("========================\n");
	printf(" Testing core namespace\n");
	printf("========================\n");

	testString();
	//testFile();
	//testThreads();
	//testCharBuf();

	//someMethod(1,2,3,4);

	benchmark();

	printf("\nFinished. Press ENTER");
	getchar();
	return 0;
}
void testString() {
	printf("==== Testing String ====\n");

	/// startsWith(string,string)
	{
		assert(String::startsWith("abcd", "abc"));
		assert(!String::startsWith("abcd", "aabc"));
	}
	/// endsWith(string,string)
	{
		assert(String::endsWith("abcd", string("bcd")));
		assert(!String::endsWith("abcd", string("aabc")));
	}
	/// contains
	{
		assert(String::contains("abcde", "cd"));
		assert(!String::contains("abcde", "Cd"));
	}
	/// trimRight
	{
		assert(String::trimRight("abc  ")=="abc");
	}
	/// trimLeft
	{
		assert(String::trimLeft("\t abc")=="abc");
	}
	/// trimBoth
	{
		assert(String::trimBoth("  \n123 \t ")=="123");
	}
	/// split(string)
	{
		auto vec = String::split(" 0  1\n2 \t3 ");
		assert(vec.size()==4);
		assert(vec[0]=="0" && vec[1]=="1" && vec[2]=="2" && vec[3]=="3");
	}
	/// split(string,char)
	{
		auto vec = String::split("0,1,2,3", ',');
		assert(vec.size()==4);
		assert(vec[0] == "0" && vec[1] == "1" && vec[2] == "2" && vec[3] == "3");
	}
	/// toString(vector<string>, string)
	{
		vector<string> vec{"0","1"};
		auto s = String::toString(vec, ", ");
		assert(s=="0, 1");
	}
	/// toString(int)
	{
		assert(String::toString(int(96))=="96");
	}
	/// toString(slong)
	{
		slong a = 911;
		string s1 = String::toString(a);
		string s2 = String::toString((slong)543);
		assert(s1 == "911");
		assert(s2 == "543");
	}
	/// toInt(string)
	{
		assert(String::toInt("436")==436);
	}
	/// toFloat(string)
	{
		assert(String::toFloat("3.14")==3.14f);
	}
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
void testFile() {
	printf("\n\nTesting File\n==================\n");	

	// todo
}

