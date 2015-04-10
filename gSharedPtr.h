#pragma once

#ifndef G_SHARED_PTR_H__
#define G_SHARED_PTR_H__

#include "gTools.h"


template <class T>
class gCustomDeallocator {
public:
	virtual void objectFreed(T* object) = 0;
};


template <class T>
class gSharedPtr {
	class gSharedPtrCounter {
		int referenceCount;
	public:
		gSharedPtrCounter() {
			referenceCount = 0;
		}

		int getCount() const {
			return referenceCount;
		}

		void increment() {
			printf("++\n");
			referenceCount++;
		}

		void decrement() {
			printf("--\n");
			assert(referenceCount);
			referenceCount--;
		}
	};


	T* object;
	gCustomDeallocator<T> *deallocator;
	gSharedPtrCounter *counter;

	void reset() {
		if (counter) {
			counter->decrement();

			if (counter->getCount() == (deallocator ? 1 : 0)) {
				if (deallocator) {
					deallocator->objectFreed(object);
				} else {
					SAFE_DELETE(object);
					SAFE_DELETE(counter);
				}
			}
		}
		object = nullptr;
		counter = nullptr;
		deallocator = nullptr;
	}

public:
	gSharedPtr() {
		this->object = nullptr;
		this->counter = nullptr;
		this->deallocator = nullptr;
	}
	~gSharedPtr() {
		reset();
	}

	gSharedPtr(const gSharedPtr<T>& other) {
		this->object = nullptr;
		this->counter = nullptr;
		this->deallocator = nullptr;
		*this = other;
	}
	gSharedPtr(gSharedPtr<T>&& other) {
		this->object = nullptr;
		this->counter = nullptr;
		this->deallocator = nullptr;
		*this = other;
	}

	gSharedPtr(T* object) {
		this->object = nullptr;
		this->counter = nullptr;
		this->deallocator = nullptr;
		*this = object;
	}

	gSharedPtr<T>& operator=(const T*& object) {
		if (object != nullptr) {
			this->object = object;
			counter = new gSharedPtrCounter();
			counter->increment();
		}
		return *this;
	}

	gSharedPtr<T>& operator=(T* object) {
		reset();
		if (object != nullptr) {
			this->object = object;
			counter = new gSharedPtrCounter();
			counter->increment();
		}
		return *this;
	}

	gSharedPtr<T>& operator=(const gSharedPtr<T>& other) {
		if (this != &other) {
			reset();
			object = other.object;
			counter = other.counter;
			deallocator = other.deallocator;
			if (counter) {
				counter->increment();
			}
		}
		return *this;
	}

	gSharedPtr<T>& operator=(gSharedPtr<T>&& other) {
		object = other.object;
		counter = other.counter;
		deallocator = other.deallocator;

		if (counter) {
			counter->increment();
		}
		return *this;
	}


	T* operator->() {
		return object;
	}

	void setCustomDeallocator(gCustomDeallocator<T> *deallocator) {
		this->deallocator = deallocator;
	}
};


#endif