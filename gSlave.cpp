#include "gSlave.h"

int slaveIndex = 0;

void gSlave::startSlaveThread(gSlaveController* controller) {
	assert(!sharedData);
	ThreadData* data = new ThreadData;
	data->slave = this;
	data->slaveIndex = ++slaveIndex;
	data->freed = false;
	data->workToDo = nullptr;
	data->controller = controller;
	this->controller = controller;
	sharedData = data;

	startSlaveThreadNative();
}



void gSlaveController::startSlaves(int slaveCount) {
	this->slaveCount = slaveCount;
	slaves = new gSlave[slaveCount];
	for (int i = 0; i < slaveCount; i++) {
		slaves[i].startSlaveThread(this);
	}
}

void gSlaveController::freeSlaves() {
	for (int i = 0; i < slaveCount; i++) {
		slaves[i].freeSlaveThread();
	}
	delete[] slaves;
	slaveCount = 0;
}

void gSlaveController::update() {
	std::vector<gSlaveWork*> works;
	controllerMutex.waitAndGetMutex();
	while (workForMainQueue.size() > 0) {
		gSlaveWork* work = workForMainQueue.front();
		workForMainQueue.pop();
		works.push_back(work);
	}
	controllerMutex.releaseMutex();

	for (unsigned i = 0; i < works.size(); i++) {
		works[i]->runOnMain();
	}
}

gSlaveWork* gSlave::getNextJobForSlave() {
	return controller->getNextJobForSlave();
}

void gSlave::slaveWorkDone(gSlaveWork* work) {
	return controller->addMainWork(work);
}


void slaveWorkDone(gSlaveWork* work);

void gSlaveController::addWork(gSlaveWork* work) {
	controllerMutex.waitAndGetMutex();
	workForSlavesQueue.push(work);
	controllerMutex.releaseMutex();
}