#ifndef HIH6130_H
#define HIH6130_H

#include "ClimateSensor.h"
#include "I2CCom.h"

#include <memory>

class HIH6130
{
public:
  HIH6130(std::shared_ptr<I2CCom> i2c);
  virtual ~HIH6130();

  virtual void init();
  virtual void reset();
  virtual void read();

  virtual uint status() const;
  virtual float temperature() const;
  virtual float humidity() const;
  virtual float pressure() const;

private:
  std::shared_ptr<I2CCom> m_i2c;

  int m_status;
  float m_temperature;
  float m_humidity;
};

#endif // HIH6130_H
