#include "_pch.h"
#include "../core.h"

using namespace core;
using std::string;
using std::vector;

void testFile();
void testFileReader();
void testFileWriter();
void testThreads();
void testCharBuffer();
void testString();
void testLog();

void benchmark();

int wmain(int argc, const wchar_t* argv[]) {
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
#endif
#ifdef _DEBUG
	printf("============== Running tests\n\n");

	testString();
	testFile();
	testThreads();
	testCharBuffer();
	testFileReader();
	testFileWriter();
	testLog();

#else
	benchmark();
#endif
	printf("\nFinished. Press ENTER");
	getchar();
	return 0;
}
void testString() {
	printf("==== Testing string.h ====\n");

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

DWORD staticFunc(void* args) {
	uint* p = (uint*)args;
	uint arg = p[0];
	printf("[%u] I am the new thread. Arg is %u...\n", 
		Thread::currentThreadId(), arg);
	printf("[%u] Thread finishing\n", Thread::currentThreadId());
	return 7;
}
void testThreads() {
	printf("==== Testing thread.h ====\n");

	printf("[Main] thread ID is %u\n", Thread::currentThreadId());
	uint arg  = 1;
	uint arg2 = 2;

	const auto lambdaFunc = [](void* args)->DWORD {
		uint* p = (uint*)args;
		uint arg = p[0];
		printf("[%u] I am the new thread. Arg is %u...\n",
			   Thread::currentThreadId(), arg);
		printf("[%u] Thread finishing\n", Thread::currentThreadId());
		return 13;
	};

	Thread t1("One", lambdaFunc, &arg);
	Thread t2("Two", staticFunc, &arg2);
	t1.start();
	t2.start();
	printf("t1 id = %u, name='%s'\n", t1.id, t1.name.c_str());
	printf("t2 id = %u, name='%s'\n", t2.id, t2.name.c_str());
	printf("t1 status before join = %u\n", t1.getStatus());
	printf("t1 exit code before join = %u\n", t1.getExitCode());
	t1.sleep(100);
	t2.yield();
	
	t1.join();
	t2.join();
	printf("[Main] I am the main thread again\n");
	printf("t1 status = %u\n", t1.getStatus());
	printf("t2 status = %u\n", t2.getStatus());
	printf("t1 exit code = %u\n", t1.getExitCode());
	printf("t2 exit code = %u\n", t2.getExitCode());

	printf("t1 cycles used = %zu\n", t1.getCyclesUsed());
}

void assertEquals(const char* a, const char* b) {
	if(a == b) return;
	if(a == nullptr || b == nullptr) assert(0);
	if(strlen(a) != strlen(b)) assert(0);
	int i = 0;
	while(a[i] && b[i]) {
		if(a[i] != b[i]) assert(0);
		i++;
	}
}
void testCharBuffer() {
	printf("==== Testing charbuffer.h ====\n");

	const auto areEqual = [](const char* a, const char* b)->bool {
		if(a == b) return true;
		if(a == nullptr || b == nullptr) return false;
		auto len = strlen(a);
		if(len != strlen(b)) return false;
		for(int i=0; i<len; i++) {
			if(a[i] != b[i]) return false;
		}
		return true;
	};

	/// constructors
	{
		printf("CharBuffer()\n");
		CharBuffer c;
		assert(c.length()==0 && c.capacity()==0 && c.empty());
	}
	{
		printf("CharBuffer(const char*,int)\n");
		CharBuffer c{"Hello", 5};
		assert(c.length()==5 && c.capacity()>=5 && !c.empty());
		printf("\tcapacity=%u\n", c.capacity());
	}
	/// c_str()
	{
		printf("c_str()\n");
		CharBuffer c{"a string"};
		assert(areEqual(c.c_str(),"a string"));
		assert(areEqual(CharBuffer{""}.c_str(), ""));
		assert(areEqual(CharBuffer{"1"}.c_str(), "1"));
	}
	/// clear
	{
		printf("clear()\n");
		CharBuffer c{"some text"};
		assert(!c.empty());
		c.clear();
		assert(c.empty() && c.length()==0 && c.capacity()>0);
	}
	/// pack
	{
		printf("pack()\n");
		CharBuffer c{"some text"};
		c.clear();
		c.pack();
		assert(c.empty() && c.length()==0 && c.capacity()==1); // space for /0
	}
	/// operator==(const char*), operator!=(const char*)
	{
		printf("operator==(const char*)\n");
		CharBuffer c{"hello"};
		assert(c=="hello");
		assert(CharBuffer{}=="");
		assert(c!="Hello");
		assert(c!="");
		assert(c!=nullptr);
		assert("hello"==c); // friend
		assert("hi"!=c);	// friend
	}
	/// operator==(string), operator!=(string)
	{
		printf("operator==(string)\n");
		CharBuffer c{"hello"};
		assert(c == string("hello"));
		assert(CharBuffer{} == string(""));
		assert(c != string("Hello"));
		assert(c != string(""));
		assert(string("hello") == c); // friend
		assert(string("hi") != c);	// friend
	}
	/// operator[] const
	{
		printf("operator[] const\n");
		CharBuffer c{"hello"};
		assert(c[0]=='h');
	}
	/// append(string)
	{
		printf("append(string)\n");
		CharBuffer c{};
		c.append(string("hello"))
		 .append(string(" there"));

		assert(c.length()==11 && c=="hello there");
	}
	/// append(const char*)
	{
		printf("append(const char*)\n");
		CharBuffer c{};
		c.append("hello")
		 .append(" there");

		assert(c.length() == 11 && c == "hello there");
	}
	/// appendFmt(const char*, ...)
	{
		printf("appendFmt(const char*, ...)\n");
		CharBuffer c{};
		c.appendFmt("%s", "hello")
		 .appendFmt("%d,", 5)
		 .appendFmt("%.2f", 3.143f);

		assert(c.length()==11 && c=="hello5,3.14");
	}
	/// contains(const char*)
	{
		printf("contains(const char*)\n");
		CharBuffer c{"hello"};
		assert(c.contains("ll"));
		assert(c.contains("hello"));
		assert(c.contains("ello"));
		assert(c.contains("llo"));
		assert(c.contains("lo"));
		assert(c.contains("o"));
		assert(!c.contains(""));
		assert(!c.contains("la"));
	}
	/// contains(string)
	{
		printf("contains(string)\n");
		CharBuffer c{"hello"};
		assert(c.contains(string("ll")));
		assert(c.contains(string("hello")));
		assert(c.contains(string("ello")));
		assert(c.contains(string("llo")));
		assert(c.contains(string("lo")));
		assert(c.contains(string("o")));
		assert(!c.contains(string("")));
		assert(!c.contains(string("la")));
	}
	/// indexOf(const char*, int) 
	{
		printf("indexOf(const char*,int)\n");
		CharBuffer c{"hello"};
		assert(c.indexOf("hello")==0);
		assert(c.indexOf("ello") == 1);
		assert(c.indexOf("llo") == 2);
		assert(c.indexOf("ll") == 2);
		assert(c.indexOf(nullptr)==-1);
		assert(c.indexOf("")==-1);
		assert(c.indexOf("hello",1)==-1);
	}
	/// indexOf(string, int)
	{
		printf("indexOf(string,int)\n");
		CharBuffer c{"hello"};
		assert(c.indexOf(string("hello")) == 0);
		assert(c.indexOf(string("")) == -1);
	}
	/// indexOf(char, int)
	{
		printf("indexOf(char,int)\n");
		CharBuffer c{"hello"};
		assert(c.indexOf('h')==0);
		assert(c.indexOf('e') == 1);
		assert(c.indexOf('l') == 2);
		assert(c.indexOf('o') == 4);
		assert(c.indexOf('z') == -1);
		assert(c.indexOf((char)0) == -1);
		assert(c.indexOf('h',1)==-1);
	}
}
void testFile() {
	printf("==== Testing file.h (File) ====\n");
	/// File::exists
	{
		printf("File::exists(string)\n");
		assert(File::exists("../LICENSE"));
	}
	/// File::size
	{
		printf("File::size(string)\n");
		assert(File::size("../LICENSE")==1089);
	}
	/// File::createTemp(string)
	string filename, renamedFilename;
	{
		printf("File::createTemp(string)\n");
		filename = File::createTemp("_temp");
		printf("\tCreated temp file '%s'\n", filename.c_str());
	}
	/// File::rename(string,string)
	{
		printf("File::rename(string,string)\n");
		renamedFilename = filename + "_renamed";
		assert(File::rename(filename, renamedFilename));
		printf("\tRenamed to '%s'\n", renamedFilename.c_str());
	}
	/// File::remove(string)
	{
		printf("File::remove(string)\n");
		assert(false==File::remove(filename));
		assert(File::remove(renamedFilename));
		printf("\tDeleted file '%s'\n", renamedFilename.c_str());
	}
	/// File::isDirectory(string)
	{
		printf("File::isDirectory(string)\n");
		assert(File::isDirectory("Debug"));
		assert(File::isDirectory("."));
		assert(File::isDirectory(".."));
		assert(false==File::isDirectory("stdafx.h"));
	}
	/// File::isFile(string)
	{
		printf("File::isFile(string)\n");
		assert(false==File::isFile("Debug"));
		assert(false == File::isFile("."));
		assert(File::isFile("../LICENSE"));
	}
	/// File::readText(string)
	{
		printf("File::readText(string)\n");
		auto text = File::readText("../LICENSE");
		assert(text.size()==1089);
		assert(text.find("MIT License")==0);
		assert(text.find("SOFTWARE.")==1089-11); // assumes it ends in \r\n
	}
	/// File::readBinary(string, [])
	{
		printf("File::readBinary(string,[])\n");
		char array[1089];
		auto num = File::readBinary("../LICENSE", array);
		assert(num==1089);
		assert(array[0] == 'M');
		assert(array[1] == 'I');
		assert(array[2] == 'T');

		char array2[16];
		auto num2 = File::readBinary("../LICENSE", array2);
		assert(num2==16);
		assert(array2[0] == 'M');
		assert(array2[1] == 'I');
		assert(array2[2] == 'T');
	}
}
void testFileReader() {
	printf("==== Testing filereader.h ====\n");

	FileReader<1024> reader{"../LICENSE"};
	printf("\tOpened file '%s'\n", reader.path.c_str());
	printf("\tSize = %lld\n", reader.size);
	assert(!reader.eof());
	assert(reader.size == 1089);

	vector<string> lines;
	while(!reader.eof()) {
		lines.push_back(reader.readLine());
	}
	printf("\tRead %llu lines\n", lines.size());
	assert(lines.size()==21);
	assert(lines.front()=="MIT License");
	assert(lines[9]=="furnished to do so, subject to the following conditions:");
	assert(lines.back()=="SOFTWARE.");
}
void testFileWriter() {
	printf("==== Testing filewriter.h ====\n");

	std::string tmpFile = File::createTemp();
	printf("\tWriting to tmp file '%s'\n", tmpFile.c_str());

	/// writeFmt(const char*,...)
	{
		FileWriter w{tmpFile, false};
		printf("writeFmt(const char*,...)\n");
		w.writeFmt("%s %d", "Hello", 3);
		assert(w.bytesWritten == 7);
	}
	assert(File::size(tmpFile)==7);
	assert(File::readText(tmpFile)=="Hello 3");

	/// test appending to existing file
	{
		FileWriter w{tmpFile, true};
		w.writeFmt("%d", 7);
		assert(w.bytesWritten == 1);
	}
	assert(File::size(tmpFile) == 8);
	assert(File::readText(tmpFile) == "Hello 37");

	/// write(ubyte)
	{
		printf("write(ubyte)\n");
		FileWriter w{tmpFile, false};
		w.write((ubyte)127);
		assert(w.bytesWritten == 1);
	}
	assert(File::size(tmpFile) == 1);
	ubyte data[1];
	File::readBinary(tmpFile, data);
	assert(data[0]==127);

	/// write(ushort)
	{
		printf("write(ushort)\n");
		FileWriter w{tmpFile, false};
		w.write((ushort)0xf7a0);
		assert(w.bytesWritten == 2);
	}
	assert(File::size(tmpFile) == 2);
	ushort data2[1];
	File::readBinary(tmpFile, data2);
	assert(data2[0] == 0xf7a0);

	/// write(uint)
	{
		printf("write(uint)\n");
		FileWriter w{tmpFile, false};
		w.write(0x80010203);
		assert(w.bytesWritten == 4);
	}
	assert(File::size(tmpFile) == 4);
	uint data3[1];
	File::readBinary(tmpFile, data3);
	assert(data3[0] == 0x80010203);

	/// write(ulong)
	{
		printf("write(ulong)\n");
		FileWriter w{tmpFile, false};
		w.write(0x80010203'fa00ee44);
		assert(w.bytesWritten == 8);
	}
	assert(File::size(tmpFile) == 8);
	ulong data4[1];
	File::readBinary(tmpFile, data4);
	assert(data4[0] == 0x80010203'fa00ee44);

	/// write(float)
	{
		printf("write(float)\n");
		FileWriter w{tmpFile, false};
		w.write(3.3f);
		assert(w.bytesWritten == 4);
	}
	assert(File::size(tmpFile) == 4);
	float data5[1];
	File::readBinary(tmpFile, data5);
	assert(data5[0] == 3.3f);

	/// write(double)
	{
		printf("write(double)\n");
		FileWriter w{tmpFile, false};
		w.write(3.7);
		assert(w.bytesWritten == 8);
	}
	assert(File::size(tmpFile) == 8);
	double data6[1];
	File::readBinary(tmpFile, data6);
	assert(data6[0] == 3.7);

	/// write(const char*)
	{
		printf("write(const char*)\n");
		FileWriter w{tmpFile, false};
		w.write("hello");
		assert(w.bytesWritten == 5);
	}
	assert(File::size(tmpFile) == 5);
	assert(File::readText(tmpFile) == "hello");

	/// write(const string)
	{
		printf("write(const string)\n");
		FileWriter w{tmpFile, false};
		w.write(string("hello"));
		assert(w.bytesWritten == 5);
	}
	assert(File::size(tmpFile) == 5);
	assert(File::readText(tmpFile) == "hello");

	File::remove(tmpFile);
}
void testLog() {
	printf("==== Testing log.h ====\n");

	/// These tests assume nothing has written to the log.log file yet

	string filename = "log.log";

	/// Log::write(const char*,...)
	{
		printf("Log::write(const char*,...)\n");
		Log::write("%s %d", "hello", 7);
		Log::close();
		assert(File::size(filename) == 9); 
		assert(File::readText(filename) == "hello 7\r\n");
	}

}
