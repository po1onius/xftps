#include "thread.h"
#include <thread>
#include <iostream>
#include <event2/event.h>
#include "task.h"
#include <unistd.h>


static void NotifyCB(evutil_socket_t fd, short which, void *arg)
{
	auto *t = (thread *)arg;
	t->Notify(fd, which);
}
void thread::Notify(evutil_socket_t fd, short which)
{
	char buf[2] = { 0 };
	unsigned int re =read(fd, buf, 1);
	if (!re)
    {
	    return;
    }
	std::cout << id << " thread " << buf << std::endl;
	task *task = nullptr;
	tasks_mutex.lock();
	if (tasks.empty())
	{
		tasks_mutex.unlock();
		return;
	}
	task = tasks.front();
	tasks.pop_front();
	tasks_mutex.unlock();
	task->Init();
}

void thread::AddTask(task *t)
{
	if (!t)
    {
	    return;
    }
	t->base = this->base;
	tasks_mutex.lock();
	tasks.push_back(t);
	tasks_mutex.unlock();
}
void thread::Activate() const
{

	unsigned int re =write(this->notify_send_fd, "c", 1);
	if (re <= 0)
	{
		std::cerr << "thread::Activate() failed!" << std::endl;
	}
}

void thread::Start()
{
	Setup();
	std::thread th(&thread::Main,this);

	th.detach();
}

bool thread::Setup()
{
    int fds[2];
	if (pipe(fds))
	{
		std::cerr << "pipe failed!" << std::endl;
		return false;
	}
	notify_send_fd = fds[1];

	event_config *ev_conf = event_config_new();
	event_config_set_flag(ev_conf, EVENT_BASE_FLAG_NOLOCK);
	this->base = event_base_new_with_config(ev_conf);
	event_config_free(ev_conf);
	if (!base)
	{
		std::cerr << "event_base_new_with_config failed in thread!" << std::endl;
		return false;
	}

	event *ev = event_new(base, fds[0], EV_READ | EV_PERSIST, NotifyCB, this);
	event_add(ev, nullptr);

	return true;
}
void thread::Main()
{
	std::cout << id << " thread::Main() begin" << std::endl;
	event_base_dispatch(base);
	event_base_free(base);

	std::cout << id << " thread::Main() end" << std::endl;
}
