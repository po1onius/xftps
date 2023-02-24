#pragma once
#include <event2/event.h>
#include <list>
#include <mutex>
class task;
class thread
{
public:

	void Start();

	void Main();

	bool Setup();

	void Notify(evutil_socket_t fd, short which);

	void Activate() const;

	void AddTask(task *t);
	thread()=default;
	~thread()=default;

	int id = 0;
private:
	int notify_send_fd = 0;
	struct event_base *base = nullptr;

	std::list<task*> tasks;
	std::mutex tasks_mutex;

};
