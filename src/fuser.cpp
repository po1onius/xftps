#include "fuser.h"
#include <iostream>
using namespace std;

void fuser::Parse(const std::string& type, const std::string& msg)
{
	cout << "fuser::Parse " << type << " " << msg << endl;
	ResCMD("230 Login successful.\r\n");
}
