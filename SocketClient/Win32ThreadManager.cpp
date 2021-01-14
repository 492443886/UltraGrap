#include "Win32ThreadManager.hpp"

using namespace std;


vector<HANDLE> threads;

unsigned nThreads = 8;
bool morePossibleWork = true;
CriticalSection consoleMtx;

// wake
HANDLE wakeEvent = NULL;

// queue of tasks to perform
typedef std::shared_ptr<Command> commandPtr;
queue<commandPtr> tasks;
CriticalSection taskMtx;
CriticalSection resultMtx;

// barrier
unsigned barrierThreshold = nThreads + 1;
unsigned barrierCount = barrierThreshold;
unsigned barrierGeneration = 0;
HANDLE barrierEvent = NULL;


void barrier() {
	unsigned gen = barrierGeneration;
	if (--barrierCount == 0) {
		++barrierGeneration;
		barrierCount = barrierThreshold;
		SetEvent(barrierEvent);

	}
	else {

		while (gen == barrierGeneration)
			WaitForSingleObject(barrierEvent, INFINITE);
	}
}

DWORD WINAPI perform_task(LPVOID) {
	/*{ CSLock lk(consoleMtx);
	cout << '[' << GetCurrentThreadId() << "]\tStarting " << endl; }*/

	barrier();

	while (morePossibleWork) {

		WaitForSingleObject(wakeEvent, INFINITE);

		while (!tasks.empty()) {
			commandPtr task;
			bool haveTask = false;
			{ // DCLP - Double Check Lock Pattern
				CSLock lk(taskMtx);
				if (!tasks.empty()) {
					task = tasks.front();
					tasks.pop();
					haveTask = true;
				}
			}


			if (haveTask) {

				task->cs = &taskMtx;
				task->execute();
				
			}
		}
	}
	return 0;
}


void Win32ThreadManager::setup()
{
	wakeEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	barrierEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	for (unsigned i = 0; i < nThreads; ++i) {
		threads.push_back(CreateThread(NULL, 0, perform_task, NULL, 0, NULL));
	}

	barrier();
}

void Win32ThreadManager::finish()
{
	while (tasks.size() != 0) {
	}
	morePossibleWork = false;
	for (size_t i = 0; i < nThreads; ++i)
		SetEvent(wakeEvent);

	// cleanup
	WaitForMultipleObjects((DWORD)threads.size(), threads.data(), TRUE, INFINITE);
	for (auto& t : threads)
		CloseHandle(t);

	CloseHandle(barrierEvent);
	CloseHandle(wakeEvent);

	threads = vector<HANDLE>();
	morePossibleWork = true;

	wakeEvent = NULL;

	barrierThreshold = nThreads + 1;
	barrierCount = barrierThreshold;
	barrierGeneration = 0;
	barrierEvent = NULL;



}

void Win32ThreadManager::addTaskHelper(commandPtr command)
{
	{ CSLock lk(taskMtx);
	tasks.push(command); }
	SetEvent(wakeEvent);
}
