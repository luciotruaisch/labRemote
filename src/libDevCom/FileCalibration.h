#ifndef FILECALIBRATION_H
#define FILECALIBRATION_H

#include "DeviceCalibration.h"

#include <string>
#include <vector>

class FileCalibration : public DeviceCalibration
{
public:
  FileCalibration(const std::string& path);
  virtual ~FileCalibration();

  virtual double calibrate(uint32_t counts);
  virtual uint32_t uncalibrate(double value);

private:
  std::vector<double> m_counts;
  std::vector<double> m_values;
};

#endif // FILECALIBRATION_H
