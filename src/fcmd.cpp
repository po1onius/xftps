#include "fcmd.h"
#include <event2/bufferevent.h>
#include <iostream>
#include <string>
using namespace std;
void fcmd::Reg(const std::string& cmd, ftask *call)
{
	if (!call)
	{
		cout << "fcmd::Reg call is null " << endl;
		return;
	}
	if (cmd.empty())
	{
		cout << "fcmd::Reg cmd is null " << endl;
		return;
	}
	if (calls.find(cmd) != calls.end())
	{
		cout << cmd << " is alreay register" << endl;
		return;
	}
	calls[cmd] = call;
	calls_del[call] = 0;
}
void fcmd::Read()
{
	char data[1024] = { 0 };
	for (;;)
	{
		unsigned int len =bufferevent_read(bev, data, sizeof(data) - 1);
		if (len == 0)
        {
		    break;
        }
		data[len] = '\0';
		cout << "Recv CMD:"<<data << flush;
		string type;
		for (int i = 0; i < len; i++)
		{
			if (data[i] == ' ' || data[i] == '\r')
            {
			    break;
            }
			type += data[i];
		}
		cout << "type is [" << type<<"]" << endl;
		if (calls.find(type) != calls.end())
		{
			ftask *t = calls[type];
			t->cmdTask = this;
			t->ip = ip;
			t->port = port;
			t->base = base;
			t->Parse(type, data);
			if (type == "PORT")
			{
				ip = t->ip;
				port = t->port;
			}
		}
		else
		{
			string msg = "200 OK\r\n";
			bufferevent_write(bev, msg.c_str(), msg.size());
		}

	}
}
void fcmd::Event(short what)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		delete this;
	}
}
bool fcmd::Init()
{
	cout << "fcmd::Init()" << endl;
	// base socket
	bufferevent * bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);
	if (!bev)
	{
		delete this;
		return false;
	}
	this->bev = bev;
	this->SetCallback();

	timeval rt = {60,0};
	bufferevent_set_timeouts(bev, &rt, nullptr);
	string msg = "220 Welcome to libevent XFtpServer\r\n";
	bufferevent_write(bev, msg.c_str(), msg.size());
	return true;
}

fcmd::~fcmd()
{
	Close();
	for (auto & ptr : calls_del)
	{
		ptr.first->Close();
		delete ptr.first;
	}
}
