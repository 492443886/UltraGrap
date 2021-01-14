#include "C11TheadMangaer.hpp"
using namespace std;


void C11ThreadManager::setup()
{
	for (unsigned i = 0; i < nThreads; ++i)
		threads.push_back(thread(&C11ThreadManager::perform_task, this));
	barrier();
}

void C11ThreadManager::finish()
{
	while (tasks.size() != 0) {
	}
	morePossibleWork = false;
	wakeCond.notify_all();
	for (auto& t : threads)
		t.join();
}

void C11ThreadManager::perform_task()
{
	barrier();
	while (morePossibleWork) {
		{ unique_lock<mutex> lk(wakeMtx);
		wakeCond.wait(lk); }
		while (!tasks.empty()) {
			commandPtr task;
			bool haveTask = false;
			{ // DCLP - Double Check Lock Pattern
				lock_guard<mutex> lk(taskMtx);
				if (!tasks.empty()) {
					task = tasks.front();
					tasks.pop();
					haveTask = true;
				}
			}
			if 
				(haveTask) {
				task->consoleMutex = &consoleMtx;
				task->execute();
			}
		}
	}
}

void C11ThreadManager::addTaskHelper(commandPtr command)
{
	{ lock_guard<mutex> lk(taskMtx);
	tasks.push(command); }
	wakeCond.notify_one();
}

void C11ThreadManager::barrier()
{
	unique_lock<mutex> lock(barrierMtx);
	unsigned gen = barrierGeneration;
	if (--barrierCount == 0) {
		++barrierGeneration;
		barrierCount = barrierThreshold;
		barrierCond.notify_all();
	}
	else {
		while (gen == barrierGeneration)
			barrierCond.wait(lock);
	}
}


