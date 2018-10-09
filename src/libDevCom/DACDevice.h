#ifndef DACDEVICE_H
#define DACDEVICE_H

#include <cstdint>
#include <vector>
#include <memory>
#include <map>

#include "DeviceCalibration.h"
#include "DummyCalibration.h"

class DACDevice
{
public:
  DACDevice(std::shared_ptr<DeviceCalibration> calibration=std::make_shared<DummyCalibration>());
  virtual ~DACDevice();

  void setCalibration(std::shared_ptr<DeviceCalibration> calibration);
  void setCalibration(uint8_t ch, std::shared_ptr<DeviceCalibration> calibration);

  virtual void setCount(uint32_t counts) =0;
  virtual void setCount(uint8_t ch, uint32_t counts) =0;
  virtual void setCount(const std::vector<uint8_t>& chs, const std::vector<uint32_t>& counts) =0;

  virtual uint32_t readCount() =0;
  virtual uint32_t readCount(uint8_t ch) =0;
  virtual void readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data) =0;

  double set(double value);
  double set(uint8_t ch, double value);
  void   set(const std::vector<uint8_t>& chs, const std::vector<double>& values);

  double read();
  double read(uint8_t ch);
  void read(const std::vector<uint8_t>& chs, std::vector<double>& data);

private:
  std::shared_ptr<DeviceCalibration> findCalibration(uint8_t ch) const;

  std::shared_ptr<DeviceCalibration> m_calibration;
  std::map<uint8_t, std::shared_ptr<DeviceCalibration>> m_channelCalibration;
};

#endif // DACDEVICE_H
