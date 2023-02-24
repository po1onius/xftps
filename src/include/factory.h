#pragma once
#include "task.h"
class factory
{
public:
	static factory* Get()
	{
		static factory f;
		return &f;
	}
	task *CreateTask();
private:
	factory()=default;
};
