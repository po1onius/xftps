#include "flist.h"
#include <iostream>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <string>
#include <unistd.h>

using namespace std;

void flist::Write()
{
	ResCMD("226 Transfer complete\r\n");
	Close();

}
void flist::Event(short what)
{
	if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR | BEV_EVENT_TIMEOUT))
	{
		cout << "BEV_EVENT_EOF | BEV_EVENT_ERROR |BEV_EVENT_TIMEOUT" << endl;
		Close();
	}
	else if(what & BEV_EVENT_CONNECTED)
	{
		cout << "flist BEV_EVENT_CONNECTED" << endl;
	}
}

void flist::Parse(const std::string& type, const std::string& msg)
{
	string resmsg;
    char pwd[FILENAME_MAX] = {0};
    cmdTask->curDir = cmdTask->curDir == "." ? getcwd(pwd, FILENAME_MAX) : cmdTask->curDir;

    std::string path = std::string(msg, msg.find_last_of(' ') + 1);
    path = std::string(path, 0, path.size() - 2);
	if (type == "PWD")
	{
		//257 "/" is current directory.
		resmsg = "257 " + cmdTask->curDir +" is current dir.\r\n";

		ResCMD(resmsg);
	}
	else if (type == "LIST")
	{
		//-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
		ConnectPORT();
		//2 1502 150
		ResCMD("150 Here comes the directory listing.\r\n");
		//string listdata = "-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n";
		string listdata = GetListData(cmdTask->curDir + "/");
		Send(listdata);
	}
	else if (type == "CWD")
	{
		//CWD test\r\n
        if(path[0] != '/')
        {
            cmdTask->curDir = cmdTask->curDir + "/" + path;
        }
        else
        {
            cmdTask->curDir = path;
        }
        //  /test/
		ResCMD("250 Directory succes chanaged.\r\n");
	}
	else if (type == "CDUP")
	{
        if(cmdTask->curDir == "/")
        {
            return;
        }
        cmdTask->curDir = std::string (cmdTask->curDir, 0, cmdTask->curDir.find_last_of('/'));
        ResCMD("250 Directory succes chanaged.\r\n");
	}
}
std::string flist::GetListData(const std::string& path)
{
	//-rwxrwxrwx 1 root group 64463 Mar 14 09:53 101.jpg\r\n
	string data;

	string cmd = "ls -l ";
	cmd += path;
	cout << "popen:" << cmd << endl;
	FILE *f = popen(cmd.c_str(), "r");
	if (!f)
    {
	    return data;
    }
	char buffer[1024] = { 0 };
	for (;;)
	{
		unsigned int len =fread(buffer, 1, sizeof(buffer) - 1, f);
		if (len == 0)
        {
		    break;
        }
		buffer[len] = '\0';
		data += buffer;
	}
	pclose(f);

	return data;
}