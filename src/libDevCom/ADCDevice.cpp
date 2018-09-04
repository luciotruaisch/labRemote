#include "ADCDevice.h"

#include <iostream>

ADCDevice::ADCDevice(std::shared_ptr<DeviceCalibration> calibration)
  : m_calibration(calibration)
{ }

ADCDevice::~ADCDevice()
{ }

void ADCDevice::setCalibration(std::shared_ptr<DeviceCalibration> calibration)
{ m_calibration=calibration; }

void ADCDevice::setCalibration(uint8_t ch, std::shared_ptr<DeviceCalibration> calibration)
{ std::cout << "save calibration for " << (uint)ch << std::endl; m_channelCalibration[ch]=calibration; }

std::shared_ptr<DeviceCalibration> ADCDevice::findCalibration(uint8_t ch) const
{
  std::cout << "saved calibration " << m_channelCalibration.size() << std::endl;
  for(auto p : m_channelCalibration)
    std::cout << (uint)p.first << " and " << (ch==p.first) << std::endl;
  std::cout << "find calibration for " << (uint)ch << std::endl;
  std::cout << (m_channelCalibration.find(ch)==m_channelCalibration.end()) << std::endl;
  return (m_channelCalibration.find(ch)==m_channelCalibration.end())?m_calibration:m_channelCalibration.at(ch);
}

double ADCDevice::read()
{ return m_calibration->calibrate(readCount()); }

double ADCDevice::read(uint8_t ch)
{ return findCalibration(ch)->calibrate(readCount(ch)); }

void ADCDevice::read(const std::vector<uint8_t>& chs, std::vector<double>& data)
{
  std::vector<uint32_t> counts(chs.size());
  readCount(chs, counts);

  data.clear();
  for(uint i=0; i<chs.size(); i++)
    data.push_back(findCalibration(chs[i])->calibrate(counts[i]));
}

