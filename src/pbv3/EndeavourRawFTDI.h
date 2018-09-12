#ifndef ENDEAVOURRAWFTDI_H
#define ENDEAVOURRAWFTDI_H

#include <memory>

#include "DeviceCom.h"
#include "EndeavourRaw.h"

class EndeavourRawFTDI : public EndeavourRaw
{
public:
  EndeavourRawFTDI();
  ~EndeavourRawFTDI();

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
  uint m_DIT_MIN   =  6*30/40,m_DIT_MID   = 14*30/40,m_DIT_MAX   = 22*30/40;
  uint m_DAH_MIN   = 29*30/40,m_DAH_MID   = 76*30/40,m_DAH_MAX   =124*30/40;
  uint m_BITGAP_MIN= 11*30/40,m_BITGAP_MID= 43*30/40,m_BITGAP_MAX= 75*30/40;

  struct ftdi_context *m_ftdi;
};

#endif //ENDEAVOURRAWFTDI_H
