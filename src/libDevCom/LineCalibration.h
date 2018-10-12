#ifndef LINECALIBRATION_H
#define LINECALIBRATION_H

#include "DeviceCalibration.h"

class LineCalibration : public DeviceCalibration
{
public:
  LineCalibration(double m, double b);
  virtual ~LineCalibration();

  virtual double calibrate(uint32_t counts);
  virtual uint32_t uncalibrate(double value);

private:
  double m_m, m_b;
};

#endif // LINECALIBRATION_H
