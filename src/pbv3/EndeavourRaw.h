#ifndef ENDEAVOURRAW_H
#define ENDEAVOURRAW_H

#include <memory>

#include "DeviceCom.h"

class EndeavourRaw
{
public:
  EndeavourRaw();

  virtual void setDitMin(uint32_t DIT_MIN) =0;
  virtual uint32_t getDitMin() =0;

  virtual void setDitMid(uint32_t DIT_MID) =0;
  virtual uint32_t getDitMid() =0;

  virtual void setDitMax(uint32_t DIT_MAX) =0;  
  virtual uint32_t getDitMax() =0;

  virtual void setDahMin(uint32_t DAH_MIN) =0;
  virtual uint32_t getDahMin() =0;

  virtual void setDahMid(uint32_t DAH_MID) =0;
  virtual uint32_t getDahMid() =0;

  virtual void setDahMax(uint32_t DAH_MAX) =0;  
  virtual uint32_t getDahMax() =0;

  virtual void setBitGapMin(uint32_t BITGAP_MIN) =0;
  virtual uint32_t getBitGapMin() =0;

  virtual void setBitGapMid(uint32_t BITGAP_MID) =0;
  virtual uint32_t getBitGapMid() =0;

  virtual void setBitGapMax(uint32_t BITGAP_MAX) =0;  
  virtual uint32_t getBitGapMax() =0;
  
  virtual void reset() =0;

  virtual void sendData(unsigned long long int data, unsigned int size) =0;

  virtual bool isError() =0;
  virtual bool isDataValid() =0;
  virtual void readData(unsigned long long int& data, unsigned int& size) =0;
};

#endif //ENDEAVOURRAW_H
