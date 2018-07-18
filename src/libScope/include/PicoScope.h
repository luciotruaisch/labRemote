#ifndef PICOSCOPE_H
#define PICOSCOPE_H

#include <vector>

class PicoScope
{
public:
  PicoScope();
  virtual ~PicoScope();

  virtual void open() =0;
  virtual void close() =0;

  virtual void setPeriod(float period) =0;
  virtual float getPeriod() =0;

  virtual void setRange(unsigned short ch, int range) =0;

  virtual void configChannels() =0;

  virtual std::vector<std::vector<float>> run() =0;

  virtual void printInfo() const =0;
};

#endif // PICOSCOPE_H
