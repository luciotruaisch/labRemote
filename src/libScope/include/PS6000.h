#ifndef PS6000_H
#define PS6000_H

#include "PicoScope.h"

#include <libps6000-1.4/ps6000Api.h>

#include <string>

struct CHANNEL_SETTINGS
{
  int16_t DCcoupled;
  int16_t range;
  int16_t enabled;
};

class PS6000 : public PicoScope
{
public:
  enum Model {
    NONE = 0,
    PS6402  = 0x6402, //Bandwidth: 350MHz, Memory: 32MS, AWG
    PS6402A = 0x6402A, //Bandwidth: 250MHz, Memory: 128MS, FG
    PS6402B = 0x6402B, //Bandwidth: 250MHz, Memory: 256MS, AWG
    PS6402C = 0x6402C, //Bandwidth: 350MHz, Memory: 256MS, AWG
    PS6402D = 0x6402D, //Bandwidth: 350MHz, Memory: 512MS, AWG
    PS6403  = 0x6403, //Bandwidth: 350MHz, Memory: 1GS, AWG
    PS6403A = 0x6403A, //Bandwidth: 350MHz, Memory: 256MS, FG
    PS6403B = 0x6403B, //Bandwidth: 350MHz, Memory: 512MS, AWG
    PS6403C = 0x6403C, //Bandwidth: 350MHz, Memory: 512MS, AWG
    PS6403D = 0x6403D, //Bandwidth: 350MHz, Memory: 1GS, AWG
    PS6404  = 0x6404, //Bandwidth: 500MHz, Memory: 1GS, AWG
    PS6404A = 0x6404A, //Bandwidth: 500MHz, Memory: 512MS, FG
    PS6404B = 0x6404B, //Bandwidth: 500MHz, Memory: 1GS, AWG
    PS6404C = 0x6404C, //Bandwidth: 350MHz, Memory: 1GS, AWG
    PS6404D = 0x6404D, //Bandwidth: 350MHz, Memory: 2GS, AWG
    PS6407  = 0x6407, //Bandwidth: 1GHz,	 Memory: 2GS, AWG
  };

  PS6000();
  virtual ~PS6000();

  virtual void open();
  virtual void close();

  virtual void setEnable(unsigned short ch, bool enable);
  virtual bool getEnable(unsigned short ch) const;

  virtual void setRange(unsigned short ch, unsigned int range);
  virtual unsigned int getRange(unsigned short ch) const;

  virtual void setPeriod(float period);
  virtual float getPeriod() const;

  virtual void configChannels();

  virtual std::vector<std::vector<float>> run();

  virtual void printInfo() const;

private:
  PICO_STATUS m_openStatus;
  int16_t m_handle =0;

  Model m_model = NONE;

  int16_t m_channelCount;
  bool m_AWG;
  int32_t m_awgBufferSize;
  CHANNEL_SETTINGS m_channelSettings[PS6000_MAX_CHANNELS];

  float m_period;
  uint32_t m_timebase=0;

  // Available ranges
  PS6000_RANGE m_firstRange;
  PS6000_RANGE m_lastRange;

  uint16_t m_availRanges [PS6000_MAX_RANGES] = {	10,
							20,
							50,
						       100,
						       200,
						       500,
						      1000,
						      2000,
						      5000,
						     10000,
						     20000,
						     50000};

  void initInfo();
};

#endif // PS6000_H
