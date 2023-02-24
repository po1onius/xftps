#include "fport.h"
#include <vector>
#include <iostream>
using namespace std;
void fport::Parse(const std::string& type, const std::string& msg)
{
	//PORT 127,0,0,1,70,96\r\n
	//PORT n1,n2,n3,n4,n5,n6\r\n
	//port = n5*256 + n6

	vector<string> vals;
	string tmp;
	for (int i = 5; i < msg.size(); i++)
	{
		if (msg[i] == ',' || msg[i] == '\r')
		{
			vals.push_back(tmp);
			tmp = "";
			continue;
		}
		tmp += msg[i];
	}
	if (vals.size() != 6)
	{
		ResCMD("501 Syntax error in parameters or arguments.");
		return;
	}
	//for (int i = 0; i < vals.size(); i++)
	//{
	//	cout << vals[i] << endl;
	//}
	ip = vals[0]+"."+vals[1] + "." + vals[2] + "." + vals[3];
	////port = n5*256 + n6
	port = atoi(vals[4].c_str()) * 256 + atoi(vals[5].c_str());
	cout << "PORT ip is " << ip << endl;
	cout << "PORT port is " << port << endl;
	ResCMD("200 PORT command successful.\r\n");

}