#include "ComIOException.h"

ComIOException::ComIOException(const std::string& msg)
  : m_msg("ComIOException: "+msg)
{ }

const char* ComIOException::what() const throw()
{ return m_msg.c_str(); }
