#ifndef ADCDEVICE_H
#define ADCDEVICE_H

#include <stdint.h>

#include <vector>

class ADCDevice
{
public:
  ADCDevice(float reference, uint32_t max);
  virtual ~ADCDevice();

  double counts2volts(uint32_t counts) const;
  
  double read();
  double readChannel(uint8_t ch);
  void readChannels(const std::vector<uint8_t>& chs, std::vector<double>& data);

  virtual uint32_t readCount() =0;
  virtual uint32_t readCountChannel(uint8_t ch) =0;
  virtual void readCountChannels(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data) =0;

private:
  float m_reference;
  uint32_t m_max;
};

#endif // ADCDEVICE_H
