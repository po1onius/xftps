#include "ftask.h"
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <iostream>
#include <cstring>
using namespace std;
void ftask::Send(const std::string& data)
{
	Send(data.c_str(),data.size());
}
void ftask::Send(const char*data,unsigned int datasize)
{
	if (!bev)
    {
	    return;
    }
	bufferevent_write(bev, data, datasize);
}
void ftask::Close()
{
	if (bev)
	{
		bufferevent_free(bev);
		bev = nullptr;
	}
	if (fp)
	{
		fclose(fp);
		fp = nullptr;
	}
}
void ftask::ConnectPORT()
{
	if (ip.empty() || port <= 0 || !base)
	{
		cout << "ConnectPORT failed ip or port or base is null" << endl;
		return;
	}
	Close();
	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	sockaddr_in sin{0};
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	evutil_inet_pton(AF_INET, ip.c_str(), &sin.sin_addr.s_addr);
	SetCallback();
	timeval rt = { 60,0 };
	bufferevent_set_timeouts(bev, &rt, 0);
	bufferevent_socket_connect(bev, (sockaddr*)&sin, sizeof(sin));
}
void ftask::ResCMD(string msg) const
{
	if (!cmdTask || !cmdTask->bev)
    {
	    return;
    }
	cout << "ResCMD:" << msg << endl;
	if (msg[msg.size() - 1] != '\n')
    {
	    msg += "\r\n";
    }
	bufferevent_write(cmdTask->bev, msg.c_str(), msg.size());
}
void ftask::SetCallback()
{
	bufferevent_setcb(bev, ReadCB, WriteCB, EventCB, this);
	bufferevent_enable(bev, EV_READ | EV_WRITE);
}

void ftask::ReadCB(struct bufferevent* bev, void *arg)
{
	auto *t = (ftask *)arg;
	t->Read();
}
void ftask::WriteCB(struct bufferevent * bev, void *arg)
{
	auto *t = (ftask *)arg;
	t->Write();
}
void ftask::EventCB(struct bufferevent *bev, short what, void *arg)
{
	auto *t = (ftask *)arg;
	t->Event(what);
}