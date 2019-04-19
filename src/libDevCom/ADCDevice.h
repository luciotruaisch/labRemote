#ifndef ADCDEVICE_H
#define ADCDEVICE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <map>

#include "DeviceCalibration.h"
#include "DummyCalibration.h"

class ADCDevice
{
public:
  ADCDevice(std::shared_ptr<DeviceCalibration> calibration=std::make_shared<DummyCalibration>());
  virtual ~ADCDevice();

  void setCalibration(std::shared_ptr<DeviceCalibration> calibration);
  void setCalibration(uint8_t ch, std::shared_ptr<DeviceCalibration> calibration);

  double read();
  double read(uint8_t ch);
  void read(const std::vector<uint8_t>& chs, std::vector<double>& data);

  virtual uint32_t readCount() =0;
  virtual uint32_t readCount(uint8_t ch) =0;
  virtual void readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data) =0;

private:
  std::shared_ptr<DeviceCalibration> findCalibration(uint8_t ch) const;
  
  std::shared_ptr<DeviceCalibration> m_calibration;
  std::map<uint8_t, std::shared_ptr<DeviceCalibration>> m_channelCalibration;
};

#endif // ADCDEVICE_H
