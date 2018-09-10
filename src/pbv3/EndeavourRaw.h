#ifndef ENDEAVOURRAW_H
#define ENDEAVOURRAW_H

#include <memory>

#include "DeviceCom.h"

class EndeavourRaw
{
public:
  EndeavourRaw();

  virtual void setDitMin(uint DIT_MIN) =0;
  virtual uint getDitMin() =0;

  virtual void setDitMid(uint DIT_MID) =0;
  virtual uint getDitMid() =0;

  virtual void setDitMax(uint DIT_MAX) =0;  
  virtual uint getDitMax() =0;

  virtual void setDahMin(uint DAH_MIN) =0;
  virtual uint getDahMin() =0;

  virtual void setDahMid(uint DAH_MID) =0;
  virtual uint getDahMid() =0;

  virtual void setDahMax(uint DAH_MAX) =0;  
  virtual uint getDahMax() =0;

  virtual void setBitGapMin(uint BITGAP_MIN) =0;
  virtual uint getBitGapMin() =0;

  virtual void setBitGapMid(uint BITGAP_MID) =0;
  virtual uint getBitGapMid() =0;

  virtual void setBitGapMax(uint BITGAP_MAX) =0;  
  virtual uint getBitGapMax() =0;
  
  virtual void reset() =0;

  virtual void sendData(unsigned long long int data, unsigned int size) =0;

  virtual bool isError() =0;
  virtual bool isDataValid() =0;
  virtual void readData(unsigned long long int& data, unsigned int& size) =0;
};

#endif //ENDEAVOURRAW_H
