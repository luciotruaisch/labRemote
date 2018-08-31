#ifndef DACDEVICE_H
#define DACDEVICE_H

#include <stdint.h>

#include <vector>

class DACDevice
{
public:
  DACDevice(float reference, uint32_t max);
  virtual ~DACDevice();

  double counts2volts(uint32_t counts) const;
  uint32_t volts2counts(double value) const;

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
  float m_reference;
  uint32_t m_max;
};

#endif // DACDEVICE_H
