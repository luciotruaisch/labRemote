#include "DACDevice.h"

#include <cmath>

DACDevice::DACDevice(std::shared_ptr<DeviceCalibration> calibration)
  : m_calibration(calibration)
{ }

DACDevice::~DACDevice()
{ }

void DACDevice::setCalibration(std::shared_ptr<DeviceCalibration> calibration)
{ m_calibration=calibration; }

void DACDevice::setCalibration(uint8_t ch, std::shared_ptr<DeviceCalibration> calibration)
{ m_channelCalibration[ch]=calibration; }

std::shared_ptr<DeviceCalibration> DACDevice::findCalibration(uint8_t ch) const
{
  return (m_channelCalibration.find(ch)==m_channelCalibration.end())?m_calibration:m_channelCalibration.at(ch);
}

double DACDevice::set(double value)
{
  uint32_t counts=m_calibration->uncalibrate(value);
  setCount(counts);
  return m_calibration->calibrate(counts);
}

double DACDevice::set(uint8_t ch, double value)
{
  uint32_t counts=findCalibration(ch)->uncalibrate(value);
  setCount(ch, counts);
  return findCalibration(ch)->calibrate(counts);
}

void DACDevice::set(const std::vector<uint8_t>& chs, const std::vector<double>& values)
{
  std::vector<uint32_t> counts;
  for(uint32_t i=0;i<chs.size();i++)
    counts.push_back(findCalibration(chs[i])->uncalibrate(values[i]));
  setCount(chs, counts);
}

double DACDevice::read()
{ return m_calibration->calibrate(readCount()); }

double DACDevice::read(uint8_t ch)
{ return findCalibration(ch)->calibrate(readCount(ch)); }

void DACDevice::read(const std::vector<uint8_t>& chs, std::vector<double>& data)
{
  std::vector<uint32_t> counts(chs.size());
  readCount(chs, counts);

  data.clear();
  for(uint32_t i=0;i<chs.size();i++)
    data.push_back(findCalibration(chs[i])->calibrate(counts[i]));
}
