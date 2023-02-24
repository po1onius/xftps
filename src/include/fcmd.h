#pragma once
#include "ftask.h"
#include <map>
class fcmd : public ftask
{
public:
	bool Init() override;
	void Read() override;
	void Event(short what) override;
	void Reg(const std::string&, ftask *call);

	fcmd()=default;
	~fcmd();
private:
	std::map<std::string, ftask*> calls;
	std::map<ftask*, int> calls_del;
};
