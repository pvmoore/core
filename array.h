#pragma once

namespace core {

template<class T,int I=8> 
class Array {
	T* array;
	uint arrayLength;
	uint numElements;
	
	inline void inflate() {
		if(arrayLength==numElements) {
			arrayLength += I;
			array = (T*)realloc(array, sizeof(T)*arrayLength);
		}
	}
	inline T none() const { 
		// Use NULL instead of nullptr in case T is a scalar type
		return NULL; 
	}
public:
	Array() : array(nullptr), arrayLength(0), numElements(0) {}
	~Array() { if(array) free(array); array = nullptr; }

	void add(T element) {
		inflate();
		array[numElements++] = element;
	}

	// todo - use memmove
	T remove(uint index) {
		if(index<numElements) {
			T thisone = array[index];
			for(uint i=index+1; i<numElements; i++) {
				array[i-1] = array[i];
			}
			numElements--;
			return thisone;
		}
		return none();
	}
	T operator[](uint index) const {
		if(index<numElements) return array[index];
		return none();
	}
	T get(uint index) const {
		if(index<numElements) return array[index];
		return none();
	}
	uint size() const { return numElements; }
	void clear() { numElements = 0; }
	void pack() {
		arrayLength = numElements;
		array = (T*)realloc(array, sizeof(T)*arrayLength);
	}
  };

} // namespace core
