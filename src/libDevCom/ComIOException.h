#ifndef COMIOEXCEPTION_H
#define COMIOEXCEPTION_H

#include <iostream>

#include "ComException.h"

class ComIOException : ComException
{
public:
  ComIOException(const std::string& msg);

  virtual const char* what() const throw();

private:
  std::string m_msg;
};

#endif // COMIOEXCEPTION_H
