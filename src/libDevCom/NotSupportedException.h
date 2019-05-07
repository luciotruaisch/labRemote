#ifndef NOTSUPPORTEDEXCEPTION_H
#define NOTSUPPORTEDEXCEPTION_H

#include <iostream>

#include "ComException.h"

class NotSupportedException : ComException
{
public:
  NotSupportedException(const std::string& msg);

  virtual const char* what() const throw();

private:
  std::string m_msg;
};

#endif // NOTSUPPORTEDEXCEPTION_H
