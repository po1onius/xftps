#include "fretr.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>
using namespace std;


void fretr::Write()
{
	if (!fp)
    {
	    return;
    }
	unsigned int len =fread(buf, 1, sizeof(buf), fp);
	if (len == 0)
	{
		ResCMD("226 Transfer complete\r\n");
		Close();
		return;
	}
		
	cout << "[" << len << "]" << flush;
	Send(buf, len);
}

void fretr::Event(short what)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		Close();
	}
	else if (what & BEV_EVENT_CONNECTED)
	{
		cout << "fretr BEV_EVENT_CONNECTED" << endl;
	}
}

void fretr::Parse(const std::string& type, const std::string& msg)
{
	unsigned int pos =msg.rfind(' ') + 1;
	string filename = msg.substr(pos, msg.size() - pos - 2);
	string path = cmdTask->curDir + "/" + filename;
	fp = fopen(path.c_str(), "rb");
	if (fp)
	{
		ConnectPORT();
		
		ResCMD("150 File OK\r\n");
		bufferevent_trigger(bev, EV_WRITE, 0);
	}
	else
	{
		ResCMD("450 file open failed!\r\n");
	}
}