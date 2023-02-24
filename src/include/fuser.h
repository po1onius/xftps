#pragma once
#include "ftask.h"
class fuser : public ftask
{
public:
	void Parse(const std::string& type, const std::string& msg) override;
	fuser()=default;
	~fuser()=default;
};
