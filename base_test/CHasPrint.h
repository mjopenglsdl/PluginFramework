#ifndef __CHASPRINT_H__
#define __CHASPRINT_H__

#include <string>

class CHasPrint 
{
public:
    virtual std::string print() {  return "Override me";  }; // = 0
    virtual ~CHasPrint() {};
    CHasPrint() {};
};

#endif