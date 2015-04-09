#include "gSlave.h"

#ifdef WINDOWS

#undef APIENTRY
#include "windows.h"

DWORD WINAPI SlaveThreadFunc(LPVOID lpParam) {
	ThreadData* data = (ThreadData*)lpParam;
	printf("slave started %d\n", data->slaveIndex);
	while (data->freed == false) {
		if (!data->workToDo) {
			data->workToDo = data->slave->getNextJobForSlave();
		}

		if (data->workToDo) {
			data->workToDo->runOnSlave();
			data->slave->slaveWorkDone(data->workToDo);
			data->workToDo = nullptr;
		}

		sleepMS(10);
	}

	SAFE_DELETE(data);
	return 0;
}

void gSlave::startSlaveThreadNative() {
	DWORD threadID;
	CreateThread(NULL, 0, SlaveThreadFunc, sharedData, 0, &threadID);
}

#endif

