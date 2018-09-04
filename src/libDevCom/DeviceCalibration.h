#ifndef DEVICECALIBRATION_H
#define DEVICECALIBRATION_H

#include <cstdint>

class DeviceCalibration
{
public:
  DeviceCalibration();
  virtual ~DeviceCalibration();

  virtual double calibrate(uint32_t counts) =0;
  virtual uint32_t uncalibrate(double value) =0;
};

#endif // DEVICECALIBRATION_H
