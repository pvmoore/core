#pragma once

//#include <string.h>
//#include "log.h"
//#include "ArrayList.h"

namespace core {

///////////////////////////////////////////////////////////////////////////////////////////// Possible key types
template<typename T> 
struct Key {
private:
	T value;
public:
	Key(T init) : value(init) {}
	uint hash() const { return value; }
	bool equals(T rhs) const { return value==rhs.value; }
};

template<> 
struct Key<const char*> {
private:
	const char* str;
	int len;
	uint h;
public:
	Key(const char* init) : str(init), len(0), h(0) {}
	uint hash() { 
		if(h==0) {
			len = (int)strlen(str);
			if(len > 16) len = 16;
			for(int i = 0; i < len; i++) {
				h = 31*h + str[i];		
			}
		}
		return h;
	}
	bool equals(const char* rhs) { 
		if(h==0) {
			len = (int)strlen(str);
		}
		int rhslen = (int)strlen(rhs);
		if(len != rhslen) return false;
		for(int i = 0; i < len; i++) {
			if(str[i] != rhs[i]) return false;
		}
		return true; 
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////

template<typename KEY, typename VALUE>
struct Entry {
	Key<KEY> key;
	VALUE value;
};

template<typename KEY, typename VALUE>
struct Bucket {
	static const int NUM_ENTRIES = 8;
	Array<Entry<KEY, VALUE>>* entries;

	Bucket() {
		printf("new Bucket (%zu bytes)\n", sizeof(Bucket));
	}
	~Bucket() { 
		// free entries here
	}
};

template<typename KEY, typename VALUE>
class HashMap {
	static const int NUM_BUCKETS = 8;
	Bucket<KEY, VALUE>* buckets;
	int numElements;

	uint getBucket(uint h) {
		h ^= (h >> 20) ^ (h >> 12);
		h = h ^ (h >> 7) ^ (h >> 4);
		return h & (NUM_BUCKETS-1);
	}
public:
	HashMap() : buckets(NULL), numElements(0) { 
		buckets = new Bucket<KEY, VALUE>[NUM_BUCKETS];
		//for(int i=0; i<NUM_BUCKETS; i++) {
		//	buckets[i] = new Bucket<KEY, VALUE>(); 
		//}
	}
	~HashMap() { 
		if(buckets) {
			//for(int i=0; i<NUM_BUCKETS; i++) {
			//	delete buckets[i];
			//}
			delete[] buckets;
		}
	}
	
	void put(int key, VALUE value) {
		uint bucket = getBucket(key);

	}
	void put(const char* key, VALUE value) {
		Key<const char*> kk(key);
		uint bucket = getBucket(kk.hash());
	}

	VALUE get(int key) const { return NULL; }
	VALUE get(const char* key) const { return NULL; }

	int	size() const { return 0; }
	bool containsKey(Key<KEY> key) const { return false; }
	//void getKeys(Array<Key<KEY>&>& dest) const {}
	//void getValues(Array<VALUE>& dest) const {}
	void clear() {
		// todo
	}
};


} // namespace core