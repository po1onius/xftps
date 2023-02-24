#include "thread_pool.h"
#include "thread.h"
#include <thread>
#include <iostream>
#include "task.h"



void thread_pool::Dispatch(task *task)
{
	if (!task)
    {
	    return;
    }
	int tid = (lastThread + 1) % threadCount;
	lastThread = tid;
	thread *t = threads[tid];

	t->AddTask(task);

	t->Activate();

}

void thread_pool::Init(int _threadCount)
{
	using namespace std::chrono_literals;
	this->threadCount = _threadCount;
	this->lastThread = -1;
	for (int i = 0; i < threadCount; i++)
	{
		auto *t = new thread();
		t->id = i + 1;
		std::cout << "Create thread " << i << std::endl;
		t->Start();
		threads.push_back(t);
		std::this_thread::sleep_for(10ms);
	}
}