#include "ADCDevice.h"

ADCDevice::ADCDevice(float reference, uint32_t max)
  : m_reference(reference), m_max(max)
{ }

ADCDevice::~ADCDevice()
{ }

double ADCDevice::counts2volts(uint32_t counts) const
{ return (double)counts/m_max*m_reference; }

double ADCDevice::read()
{ return counts2volts(readCount()); }

double ADCDevice::read(uint8_t ch)
{ return counts2volts(readCount(ch)); }

void ADCDevice::read(const std::vector<uint8_t>& chs, std::vector<double>& data)
{
  std::vector<uint32_t> counts(chs.size());
  readCount(chs, counts);

  data.clear();
  for(uint32_t count : counts)
    data.push_back(counts2volts(count));
}

