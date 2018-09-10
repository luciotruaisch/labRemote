#ifndef ENDEAVOURCOMEXCEPTION_H
#define ENDEAVOURCOMEXCEPTION_H

#include <stdarg.h>

#include <string>

#include "ComException.h"

class EndeavourComException : ComException
{
public:
  EndeavourComException(const std::string& msg);
  EndeavourComException(const char *format, ...);

  virtual const char* what() const throw();

private:
  std::string m_msg;
};

#endif // ENDEAVOURCOMEXCEPTION_H
