#pragma once
#include "ftask.h"

class fstore : public ftask
{
public:
	void Parse(const std::string& type,const std::string& msg) override;
	void Read() override;
	void Event(short what) override;
private:
	FILE *fp = nullptr;
	char buf[1024] = { 0 };
};
