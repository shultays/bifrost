#pragma once

#ifndef G_MUTEX_H__
#define G_MUTEX_H__

#include "gTools.h" 


class gMutex {
	void* mutex;

public:

	gMutex();

	~gMutex();

	void waitAndGetMutex();


	bool waitAndGetMutex(int maxWaitInMS);


	bool tryToGetMutex();


	void releaseMutex();
};


#endif