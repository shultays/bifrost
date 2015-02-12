#ifndef FIXED_SIZED_PRIORITY_ARRAY_H__
#define FIXED_SIZED_PRIORITY_ARRAY_H__

#include "gTools.h"

const int max_priority = 10000;

template <class T, int N>
class FixedSizedPriorityArray{

	struct Element{
		int p;
		T item;
	};

	Element data[N];
	int count;

	void fixInsert(int i){
		while (true){
			if (i == 0) break;
			if (data[i].p > data[i - 1].p) break;
			int j = i - 1;
			int p = data[j].p;
			j--;
			while (j >= 0 && data[j].p == p) j--;

			swapt(data[j + 1], data[i]);
			i = j + 1;
		}
	}

	void fixRemove(int i){
		while (true){
			if (i == count-1) break;
			if (data[i].p < data[i + 1].p) break;
			int j = i + 1;
			int p = data[j].p;
			j++;
			while (j < count && data[j].p == p) j++;

			swapt(data[j - 1], data[i]);
			i = j - 1;
		}
	}

	int fix(int i){
		if (i > 0 && data[i].p < data[i - 1].p) fixInsert(i);
		if (i < count -1 && data[i].p < data[i + 1].p) fixRemove(i);
	}

public:
	FixedSizedPriorityArray(){
		count = 0;
	}

	void clear(){
		count = 0;
	}

	void insert(T item, int priority){
		assert(count < N);
		data[count].item = item;
		data[count].p = priority;
		count++;
		fixInsert(count-1);
	}

	void insert(T item){
		insert(item, max_priority);
	}

	void remove(T item){
		for (int i = 0; i < count; i++){
			if (data[i].item == item){
				removeAtIndex(i);
				return;
			}
		}
	}
	void removeAtIndex(int i){
		data[i].p = max_priority + 1;
		fixRemove(i);
		count--;
	}

	void changePriority(T item, int newPriority){
		for (int i = 0; i < count; i++){
			if (data[i].item == item){
				changePriorityAtIndex(i, newPriority);
				return;
			}
		}
	}
	int changePriorityAtIndex(int i, int newPriority){
		data[i].p = newPriority;
		fix(i);
	}
	inline T operator[](int i){
		return data[i].item;
	}

	int size(){
		return count;
	}
};

#endif