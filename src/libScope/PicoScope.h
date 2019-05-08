#ifndef PICOSCOPE_H
#define PICOSCOPE_H

#include <vector>

#include <stdint.h>

class PicoScope
{
public:
  enum Direction {Rising, Falling};

  PicoScope();
  virtual ~PicoScope();

  //
  // Initialization
  virtual void open() =0;
  virtual void close() =0;

  //
  // Trigger configuration
  virtual void setTrigger  (uint8_t ch, float threshold, Direction direction) =0;
  virtual void unsetTrigger(uint8_t ch) =0;

  virtual void setTriggerPosition(float percentage) =0;
  virtual float getTriggerPosition() =0;

  //
  // Channel configurations
  virtual void setEnable(unsigned short ch, bool enable) =0;
  virtual bool getEnable(unsigned short ch) const =0;

  virtual void setRange(unsigned short ch, unsigned int range) =0;
  virtual unsigned int getRange(unsigned short ch) const =0;

  virtual void setPeriod(float period) =0;
  virtual float getPeriod() const =0;

  virtual void configChannels() =0;

  //
  // Running stuff
  virtual void     setSamples(uint32_t samples) =0;
  virtual uint32_t getSamples() =0;

  virtual void run(std::vector<std::vector<float>> &results) =0;
  virtual void run(std::vector<std::vector<int16_t>> &results) =0;

  virtual void printInfo() const =0;
};

#endif // PICOSCOPE_H
