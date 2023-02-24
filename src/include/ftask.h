#pragma once
#include "task.h"
#include <string>
class ftask : public task
{
public:

	std::string curDir = ".";

	std::string ip;
	int port = 0;
	
	ftask *cmdTask = nullptr;

	virtual void Parse(const std::string& type, const std::string& msg) {}
	void ResCMD(std::string msg) const;


	void Send(const std::string& data);
	void Send(const char* data,unsigned int datasize);

	void ConnectPORT();
	void Close();
	virtual void Read() {}
	virtual void Write() {}
	virtual void Event(short what) {}
	void SetCallback();
	bool Init() override { return true; }
protected:
	static void ReadCB(struct bufferevent * bev, void *arg);
	static void WriteCB(struct bufferevent * bev, void *arg);
	static void EventCB(struct bufferevent *bev, short what, void *arg);
	struct bufferevent *bev = nullptr;
	FILE *fp = nullptr;

};
