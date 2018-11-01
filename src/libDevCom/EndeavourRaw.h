#ifndef ENDEAVOURRAW_H
#define ENDEAVOURRAW_H

#include <memory>

#include "DeviceCom.h"

class EndeavourRaw
{
public:
  EndeavourRaw(std::shared_ptr<DeviceCom> fpgaCom);

  void reset();

  void sendData(unsigned long long int data, unsigned int size);

  bool isError();
  bool isDataValid();
  void readData(unsigned long long int& data, unsigned int& size);

private:
  std::shared_ptr<DeviceCom> m_fpgaCom;
};

#endif //ENDEAVOURRAW_H
