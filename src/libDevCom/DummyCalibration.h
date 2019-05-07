#ifndef DUMMYCALIBRATION_H
#define DUMMYCALIBRATION_H

#include "DeviceCalibration.h"

class DummyCalibration : public DeviceCalibration
{
public:
  virtual double calibrate(uint32_t counts);
  virtual uint32_t uncalibrate(double value);
};

#endif // DUMMYCALIBRATION_H
