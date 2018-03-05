#pragma once

namespace core {

class String;
typedef std::shared_ptr<String> StringRef;

/// 
/// immutable String implementation
///
class String {
	char* chararray;
	uint chararrayLength;
	uint offset;
	uint count;

	// reference counting
	struct ChararrayReference {
		uint chararrayReferences;
		ChararrayReference() : chararrayReferences(2) {}
	} *reference;
	
	/// findme guaranteed to be non-NULL
	int _indexOf(const char* const findme, uint cLen, uint start, uint end) const {
		if(start>=count || start>=end) return -1;
		if(cLen==0 || cLen>count-start) return -1;
		if(end>count) end = count;

		//return core_indexof(chararray+offset, findme, count, cLen);

		start += offset;
		end += offset;
		end -= (cLen-1);

		uint cPos = 0;
		for(uint i=start; i<end; i++) {
			char cc = chararray[i];
			if(cc==findme[cPos]) {
				if(++cPos==cLen) return i-cLen+1;
			} else {
				cPos = 0;
			}
		}
		return -1;
	}
	/// findme guaranteed to be non-NULL
	int _lastIndexOf(const char* const findme, uint cLen, uint start, uint end) const {
		if(start>=count || start>=end) return -1;
		if(cLen==0 || cLen>count-start) return -1;
		if(end>count) end = count;
		start += offset;
		end += offset;

		for(int i=end-cLen; i>=(int)start; i--) {
			const char* const p = chararray+i;
			uint cPos = 0;
			for(uint n=0; n<cLen; n++) {
				if(p[n]==findme[n]) {
					if(++cPos==cLen) return i;
				} else cPos = 0;
			}
		}
		return -1;
	}
	uint _parseUint(char* const buf, uint n) const {
		uint pos = 15;
		do{
			uint rem = (uint)(n%10);
			n /= 10;
			buf[--pos] = '0'+rem;
		}while(n>0);
		return pos;
	}
	uint _parseUlong(char* const buf, ulong n) const {
		uint pos = 30;
		do{
			uint rem = (uint)(n%10);
			n /= 10;
			buf[--pos] = '0'+rem;
		}while(n>0);
		return pos;
	}
	void _init(const char* const chars, uint length) {
		if(length==0) {
			this->chararrayLength	= 0;
			this->chararray			= new char[1];
		} else {
			this->chararrayLength = length;
			this->chararray = new char[length+1];
			memcpy(this->chararray, chars, length);
		}
		this->count = length;
		this->chararray[length] = 0;
	}
	void _delete() {
		if(!reference || --(reference->chararrayReferences)==0) {
			delete[] chararray;
			delete reference;
		} 
		reference = nullptr;
		chararray = nullptr;
	}
public:
	////////////////////////////////////////////////////////////////////////////////////////////////////// constructors
	String() : chararray(nullptr), chararrayLength(0), offset(0), count(0), reference(nullptr) {
		printf("{default constructor}\n");
		_init(nullptr, 0);
	}
	explicit String(bool b) : reference(nullptr), offset(0) {
		printf("{bool constructor}\n");
		if(b) {	
			_init("true", 4);
		} else {
			_init("false", 5);
		}
	}
	explicit String(char c) : reference(nullptr), offset(0) {
		printf("{char constructor}\n");
		_init(&c, 1);
	}
	explicit String(sint n) : reference(nullptr), offset(0) {
		printf("{sint constructor}\n");
		bool neg = false;
		if(n<0) {
			neg = true;
			n = -n;
		}
		char temp[15];
		uint pos = _parseUint(temp, n);
		if(neg) temp[--pos] = '-';
		_init(temp+pos, 15-pos);
	}
	explicit String(uint n) : reference(nullptr), offset(0) {
		printf("{uint constructor}\n");
		char temp[15];
		uint pos = _parseUint(temp, n);
		_init(temp+pos, 15-pos);
	}
	explicit String(slong n) : reference(nullptr), offset(0) {
		printf("{slong constructor}\n");
		bool neg = false;
		if(n<0) {
			neg = true;
			n = -n;
		}
		char temp[30];
		uint pos = _parseUlong(temp, n);
		if(neg) temp[--pos] = '-';
		_init(temp+pos, 30-pos);
	}
	explicit String(ulong n) : reference(nullptr), offset(0) {
		printf("{ulong constructor}\n");
		char temp[30];
		uint pos = _parseUlong(temp, n);
		_init(temp+pos, 30-pos);
	}
	explicit String(float d) : reference(NULL), offset(0) {
		printf("{float constructor}\n");
		char temp[32];
		sprintf(temp,"%g",d);
		_init(temp, (uint)strlen(temp));
	}
	explicit String(double d) : reference(nullptr), offset(0) {
		printf("{double constructor}\n");
		char temp[32];
		sprintf(temp,"%g",d);
		_init(temp, (uint)strlen(temp));
	}
	String(const char* const str, uint start=0, uint end=MAX_UINT) : reference(nullptr), offset(0) {
		uint len = (uint)strlen(str);
		if(end>len) end = len;
		printf("{char* constructor '%s' chars %u to %u}\n", str, start, end);

		if(start>=end) {
			_init(nullptr, 0);
			return;
		}
		_init(str+start, end-start);
	}
	//////////////////////////////////////////////////////////////////////////////////////////// copy constructors
	String(String& str, uint start=0, uint end=MAX_UINT) {
		if(end>str.count) end = str.count;
		printf("{copy String& - reusing existing chararray '%s' chars %u to %u}\n", str.chararray, start, end);
		
		if(start>=end) {
			_init(nullptr, 0);
			return;
		}

		this->reference = str.reference;
		if(!this->reference) {
			this->reference = new ChararrayReference();
			str.reference = this->reference;
		} else {
			this->reference->chararrayReferences++;
		}
		this->offset			= str.offset + start;
		this->count				= end-start;
		this->chararrayLength	= str.chararrayLength;
		this->chararray			= str.chararray;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////// move constructor
	String(String&& str) :
		chararray(str.chararray),
		chararrayLength(str.chararrayLength),
		offset(str.offset),
		count(str.count) 
	{
		printf("{copy String&&}\n");
		str.chararray = nullptr;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////// destructor
	~String() {
		_delete();
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////// public methods
	const char* const cStr() {
		if(offset==0 && count==chararrayLength) {
			// just return chararray
			return const_cast<const char* const>(chararray);
		}

		// hm. Is this right?
		if(reference && reference->chararrayReferences==1) {
			// we don't need this reference any more
			delete reference;
			reference = nullptr;
		}
		
		if(!reference) {
			// no-one else is looking at our chararray - we can just modify it in place
			// PS. This is rubbish
			chararray[offset+count] = 0;
			return const_cast<const char* const>(chararray+offset);
		}
		// make a copy 
		char* copy = new char[count+1];
		//pvmoore core_memcpy(copy, chararray+offset, count);
		copy[count] = 0;
		
		// we are no longer referencing the old chararray
		_delete();
		chararray = copy;
		chararrayLength = count;
		offset = 0;
		return const_cast<const char* const>(chararray);
	}
	uint length() const { 
		return count; 
	}
	int indexOf(const String& findme, uint start=0, uint end=MAX_UINT) const {
		if(findme.count==0) return -1;
		return _indexOf(findme.chararray+findme.offset, findme.count, start, end);
	}
	int indexOf(const char* const findme, uint start=0, uint end=MAX_UINT) const {
		if(findme==nullptr) return -1;
		return _indexOf(findme, (uint)strlen(findme), start, end);
	}
	int lastIndexOf(const String& findme, uint start=0, uint end=MAX_UINT) const {
		if(findme.count==0) return -1;
		return _lastIndexOf(findme.chararray+findme.offset, findme.count, start, end);
	}
	int lastIndexOf(const char* const findme, uint start=0, uint end=MAX_UINT) const {
		if(findme==nullptr) return -1;
		return _lastIndexOf(findme, (uint)strlen(findme), start, end); 
	}
	bool contains(const String& text, uint start=0, uint end=MAX_UINT) const {
		return indexOf(text, start, end)!=-1;
	}
	bool contains(const char* text, uint start=0, uint end=MAX_UINT) const {
		return indexOf(text, start, end)!=-1;
	}
	StringRef lowerCased()  {
		StringRef copy = std::make_shared<String>(*this);
		//StringRef copy(new String(this));
		
		/// TODO - this is ascii only and it's rubbish
		for(uint i=copy->offset; i<copy->count; i++) {
			if(copy->chararray[i] >= 'A' && copy->chararray[i] <= 'Z') copy->chararray[i] += 32;	
		}

		return copy;
	}
	// does this work? who cleans up the returned String? or is it a copy?
	String substring(uint start, uint end=MAX_UINT) {
		if(end>chararrayLength) end = chararrayLength;
		if(start>=end) start = end = 0;
		//return String( *(const_cast<String*>(this)), start, end);
		return String(*this, start, end);
	}
	//String replace(const char* const replaceme, const char* const withme, uint start=0, uint end=MAX_UINT) const {
		// FIXME
		/*size_t wlen = strlen(withme);
		  size_t rlen = strlen(replaceme);
		  //
		  if(start<0) start=0;
		  if(end>len) end = len;
		  char* temp = new char[len+1];
		  while(true) {
			char* i = strstr(chararray,replaceme);
			if(i!=NULL) {
      
			}
		  }
		  //return Kestrel::HEAP.Alloc(new String(*this),false,"");
		  */
		//return *this;
	//}
	//////////////////////////////////////////////////////////////////////////////////// operators
	//operator const char*() {
	//	printf("cast to char*\n");
		// this could cause a problem if the chararray is deleted
	//	return cStr();
	//}
	bool operator==(String& rhs) const {
		printf("{%s(%u)==%s(%u)}\n", chararray, count, rhs.chararray+rhs.offset, rhs.count);
		if(this==&rhs) return true;
		if(count!=rhs.count) return false; 
		return memcmp(chararray+offset, rhs.chararray+rhs.offset, count)==0;
	}
	bool operator==(const char* const rhs) const {
		printf("{%s(%u)==%s(%u)}\n", chararray, count, rhs, (int)strlen(rhs));
		uint length = (uint)strlen(rhs);
		if(count!=length) return false;
		return memcmp(chararray+offset, rhs, count)==0;
	}
	bool operator!=(String& rhs) const {
		return !operator==(rhs);
	}
	bool operator!=(const char* const rhs) const {
		return !operator==(rhs);
	}
	/// copy assignment --> this = rhs
	String& operator=(String& rhs) { 	
		if(this != &rhs) {
			printf("assign - reusing chararray '%s'\n", rhs.chararray);
			// delete our chararray if we have the only reference
			_delete();

			// point to rvalue chararray
			this->reference = rhs.reference;
			if(!this->reference) {
				this->reference = new ChararrayReference();
				rhs.reference = this->reference;
			} else {
				this->reference->chararrayReferences++;
			}
			this->chararrayLength	= rhs.chararrayLength;
			this->chararray			= rhs.chararray;
			this->offset			= rhs.offset;
			this->count				= rhs.count;
		}
		return *this;
	}
	/// copy assignment to temporary -> this = temp
	String& operator=(String&& temp) {
		printf("assign to temporary\n");
		// delete our chararray if we have the only reference
		_delete();

		// point to rvalue chararray
		this->reference = new ChararrayReference();
		temp.reference = this->reference;

		this->chararrayLength	= temp.chararrayLength;
		this->chararray			= temp.chararray;
		this->offset			= temp.offset;
		this->count				= temp.count;
		return *this;
	}
	/*
	String& operator=(const char* const rvalue) { 
		// copy assignment --> this = rvalue 
		delete[] chararray;
		len = (uint)strlen(rvalue);
		chararray = new char[len+1];
		strcpy(chararray, rvalue);
		return *this;
	}*/
	char operator[](int i) const {
		return (chararray+offset)[i];
	}
	/*
	void* operator new(size_t bytes) {
		printf("new String\n",bytes);

		//return ::operator new(bytes);
	}
	void operator delete(void* p) {
		printf("delete String\n",(int)p);
		::operator delete(p);
	}
	*/
	/*
	void* operator new[](size_t bytes) {
		printf("new[] %u bytes\n",bytes);
		return ::operator new[](bytes);
	}
	void operator delete[](void* p) {
		printf("delete[] %u\n",(int)p);
		::operator delete[](p);
	}
	*/
	//////////////////////////////////////////////////////////////////////////////////////// friends
/*
	friend String operator+(char, String&);
	friend String operator+(String&, char);
	friend String operator+(String&, String&);  
	friend String operator+(bool, String&);    
	friend String operator+(String&, bool);
	friend String operator+(double, String&);
	friend String operator+(String&, double);  
	friend String operator+(char*, String&);
	friend String operator+(String&, char*);
	friend String operator+(long, String&);
	friend String operator+(String&, long);    
*/
};

} // namespace core