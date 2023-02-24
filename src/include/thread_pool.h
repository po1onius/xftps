#pragma once
#include <vector>
class thread;
class task;
class thread_pool
{
public:
	static thread_pool* Get()
	{
		static thread_pool p;
		return &p;
	}
	void Init(int _threadCount);

	void Dispatch(task *task);
private:
	int threadCount = 0;
	int lastThread = -1;
	std::vector<thread *> threads;
	thread_pool() = default;
};

