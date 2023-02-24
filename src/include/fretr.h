#pragma once
#include "ftask.h"
class fretr : public ftask
{
public:
	void Parse(const std::string& type, const std::string& msg) override;
	void Write() override;
	void Event(short what) override;

private:
	FILE *fp = nullptr;
	char buf[1024] = { 0 };
};

