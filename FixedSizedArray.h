#ifndef FIXED_SIZED_ARRAY_H__
#define FIXED_SIZED_ARRAY_H__
#include "gTools.h"
template <class T, int N>
class FixedSizedArray{
	T data[N];
	int count;
public:
	
	FixedSizedArray(){
		count = 0;
	}
	void clear(){
		count = 0;
	}
	void insert(T item){
		assert(count < N);
		data[count++] = item;
	}
	void remove(T item){
		for (int i = 0; i < count; i++){
			if (data[i] == item){
				data[i] = data[count - 1];
				count--;
				return;
			}
		}
	}

	void removeAtIndex(int i){
		data[i] = data[count - 1];
		count--;
	}
	inline T operator[](int i){
		return data[i];
	}

	int size(){
		return count;
	}
};

#endif