#ifndef ENDEAVOURRAW_H
#define ENDEAVOURRAW_H

#include <memory>

#include "DeviceCom.h"

class EndeavourRaw
{
public:
  EndeavourRaw(std::shared_ptr<DeviceCom> fpgaCom);

  void setDitMin(uint DIT_MIN);
  uint getDitMin();

  void setDitMid(uint DIT_MID);
  uint getDitMid();

  void setDitMax(uint DIT_MAX);  
  uint getDitMax();

  void setDahMin(uint DAH_MIN);
  uint getDahMin();

  void setDahMid(uint DAH_MID);
  uint getDahMid();

  void setDahMax(uint DAH_MAX);  
  uint getDahMax();

  void setBitGapMin(uint BITGAP_MIN);
  uint getBitGapMin();

  void setBitGapMid(uint BITGAP_MID);
  uint getBitGapMid();

  void setBitGapMax(uint BITGAP_MAX);  
  uint getBitGapMax();
  
  void reset();

  void sendData(unsigned long long int data, unsigned int size);

  bool isError();
  bool isDataValid();
  void readData(unsigned long long int& data, unsigned int& size);

private:
  std::shared_ptr<DeviceCom> m_fpgaCom;
};

#endif //ENDEAVOURRAW_H
