#ifndef ENDEAVOURRAWUIO_H
#define ENDEAVOURRAWUIO_H

#include <memory>

#include "DeviceCom.h"
#include "EndeavourRaw.h"

class EndeavourRawUIO : public EndeavourRaw
{
public:
  EndeavourRawUIO(std::shared_ptr<DeviceCom> fpgaCom);

  void setDitMin(uint32_t DIT_MIN);
  uint32_t getDitMin();

  void setDitMid(uint32_t DIT_MID);
  uint32_t getDitMid();

  void setDitMax(uint32_t DIT_MAX);  
  uint32_t getDitMax();

  void setDahMin(uint32_t DAH_MIN);
  uint32_t getDahMin();

  void setDahMid(uint32_t DAH_MID);
  uint32_t getDahMid();

  void setDahMax(uint32_t DAH_MAX);  
  uint32_t getDahMax();

  void setBitGapMin(uint32_t BITGAP_MIN);
  uint32_t getBitGapMin();

  void setBitGapMid(uint32_t BITGAP_MID);
  uint32_t getBitGapMid();

  void setBitGapMax(uint32_t BITGAP_MAX);  
  uint32_t getBitGapMax();
  
  void reset();

  void sendData(unsigned long long int data, unsigned int size);

  bool isError();
  bool isDataValid();
  void readData(unsigned long long int& data, unsigned int& size);

private:
  std::shared_ptr<DeviceCom> m_fpgaCom;
};

#endif //ENDEAVOURRAWUIO_H
