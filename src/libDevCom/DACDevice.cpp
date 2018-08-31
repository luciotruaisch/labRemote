#include "DACDevice.h"

#include <cmath>

DACDevice::DACDevice(float reference, uint32_t max)
  : m_reference(reference), m_max(max)
{ }

DACDevice::~DACDevice()
{ }

double DACDevice::counts2volts(uint32_t counts) const
{ return (double)counts/m_max*m_reference; }

uint32_t DACDevice::volts2counts(double value) const
{ return round(value*m_max/m_reference); }

double DACDevice::set(double value)
{
  uint32_t counts=volts2counts(value);
  setCount(counts);
  return counts2volts(counts);
}

double DACDevice::set(uint8_t ch, double value)
{
  uint32_t counts=volts2counts(value);
  setCount(ch, counts);
  return counts2volts(counts);
}

void DACDevice::set(const std::vector<uint8_t>& chs, const std::vector<double>& values)
{
  std::vector<uint32_t> counts;
  for(double value : values)
    counts.push_back(volts2counts(value));
  setCount(chs, counts);
}

double DACDevice::read()
{ return counts2volts(readCount()); }

double DACDevice::read(uint8_t ch)
{ return counts2volts(readCount(ch)); }

void DACDevice::read(const std::vector<uint8_t>& chs, std::vector<double>& data)
{
  std::vector<uint32_t> counts(chs.size());
  readCount(chs, counts);

  data.clear();
  for(uint32_t count : counts)
    data.push_back(counts2volts(count));
}
