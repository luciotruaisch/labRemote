#include "NotSupportedException.h"

NotSupportedException::NotSupportedException(const std::string& msg)
  : m_msg("NotSupportedException: "+msg)
{ }

const char* NotSupportedException::what() const throw()
{ return m_msg.c_str(); }
