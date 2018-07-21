#ifndef PICOSCOPE_H
#define PICOSCOPE_H

#include <vector>

class PicoScope
{
public:
  PicoScope();
  virtual ~PicoScope();

  //
  // Initialization
  virtual void open() =0;
  virtual void close() =0;

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
  virtual std::vector<std::vector<float>> run() =0;

  virtual void printInfo() const =0;
};

#endif // PICOSCOPE_H
