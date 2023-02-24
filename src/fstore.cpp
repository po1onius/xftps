#include "fstore.h"
#include <iostream>
#include <event2/bufferevent.h>
#include <event2/event.h>
using namespace std;

void fstore::Parse(const std::string& type,const std::string& msg)
{
	unsigned int pos =msg.rfind(' ') + 1;
	string filename = msg.substr(pos, msg.size() - pos - 2);
	string path = cmdTask->curDir + "/" + filename;
	fp = fopen(path.c_str(), "wb");
	if (fp)
	{
		ConnectPORT();

		ResCMD("125 File OK\r\n");
		bufferevent_trigger(bev, EV_READ, 0);
	}
	else
	{
		ResCMD("450 file open failed!\r\n");
	}
}
void fstore::Read()
{
	if (!fp)
    {
	    return;
    }
	for (;;)
	{
		unsigned int len =bufferevent_read(bev, buf, sizeof(buf));
		if (len <= 0)
        {
		    return;
        }
		unsigned int size =fwrite(buf, 1, len, fp);
		cout << "<"<<len<<":"<<size << ">" << flush;
	}
}
void fstore::Event(short what)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "fstore BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		Close();
		ResCMD("226 Transfer complete\r\n");
	}
	else if (what&BEV_EVENT_CONNECTED)
	{
		cout << "fstore BEV_EVENT_CONNECTED" << endl;
	}
}