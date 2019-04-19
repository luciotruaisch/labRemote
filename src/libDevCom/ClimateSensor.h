#ifndef CLIMATESENSOR_H
#define CLIMATESENSOR_H

class ClimateSensor
{
public:
  ClimateSensor();
  virtual ~ClimateSensor();

  virtual void init() =0;
  virtual void reset() =0;
  virtual void read() =0;

  virtual float temperature() const =0;
  virtual float humidity() const =0;
  virtual float pressure() const =0;
};

#endif // CLIMATESENSOR_H
