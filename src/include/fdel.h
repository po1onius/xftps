#ifndef XFTPS_FDEL_H
#define XFTPS_FDEL_H

#include "ftask.h"
class fdel : public ftask
{
public:
    void Parse(const std::string& type, const std::string& msg) override;
private:
};

#endif //XFTPS_FDEL_H
