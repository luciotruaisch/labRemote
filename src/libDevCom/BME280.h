#ifndef BME280_H
#define BME280_H

#include <memory>

#include "ClimateSensor.h"
#include "SPICom.h"

class BME280 : public ClimateSensor
{
public:
  BME280(std::shared_ptr<SPICom> devcom);
  virtual ~BME280();

  virtual void init();
  virtual void reset();

  virtual float temperature();
  virtual float humidity();
  virtual float pressure();

  void dump();

 private:
  std::shared_ptr<SPICom> m_devcom;

  // Calibration constants
  void load_calibration();

  ushort m_dig_T1;
  short m_dig_T2;
  short m_dig_T3;

  ushort m_dig_P1;
  short m_dig_P2;
  short m_dig_P3;
  short m_dig_P4;
  short m_dig_P5;
  short m_dig_P6;
  short m_dig_P7;
  short m_dig_P8;
  short m_dig_P9;

  uint8_t m_dig_H1;
  int16_t m_dig_H2;
  uint8_t m_dig_H3;
  int16_t m_dig_H4;
  int16_t m_dig_H5;
  int8_t  m_dig_H6;

  // Shared data
  int m_t_fine;
};

#endif // BME280_H
