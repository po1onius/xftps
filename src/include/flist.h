#pragma once
#include "ftask.h"
class flist : public ftask
{
public:
	void Parse(const std::string& type, const std::string& msg) override;
	void Write() override;
	void Event(short what) override;
private:
	std::string GetListData(const std::string& path);
};
