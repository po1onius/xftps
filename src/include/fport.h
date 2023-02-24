#pragma once
#include "ftask.h"
class fport :
	public ftask
{
public:
	void Parse(const std::string& type, const std::string& msg) override;
};

