#include "PS6000.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include <math.h>

PS6000::PS6000()
  : PicoScope()
{ }

PS6000::~PS6000()
{
  // Cleanup the buffers
  for(uint ch=0;ch<4;ch++)
    {
      if(m_buffers[ch]!=nullptr)
	{
	  delete[] m_buffers[ch];
	  m_buffers[ch]=nullptr;
	}
    }
 }

void PS6000::open()
{
  m_openStatus = ps6000OpenUnit(&m_handle, 0);

  if(m_openStatus==PICO_OK || m_openStatus==PICO_USB3_0_DEVICE_NON_USB3_0_PORT)
    ;
  else if(m_openStatus==PICO_NOT_FOUND)
    throw std::string("Picoscope not found");
  else
    throw std::string("Unable to open Picoscope due to unknown error");

  initInfo();
  configChannels();
}

void PS6000::close()
{
  ps6000CloseUnit(m_handle);
  m_handle=0;
}

void PS6000::initInfo()
{
  int16_t r=20;
  int8_t line[20];

  // 
  // Read variant type
  ps6000GetUnitInfo(m_handle, line, sizeof(line), &r, PICO_VARIANT_INFO);

  int32_t variant;
  std::stringstream ss;
  ss << line;
  ss >> std::hex >> variant;

  //
  // Configure based on variant
  switch(variant)
    {
    case PS6402:
      m_model         = PS6402;
      m_firstRange    = PS6000_50MV;
      m_lastRange     = PS6000_20V;
      m_channelCount  = 4;
      m_AWG           = true;
      m_awgBufferSize = MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6402A:
      m_model		= PS6402A;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = false;
      m_awgBufferSize = 0;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6402B:
      m_model		= PS6402B;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_awgBufferSize = MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6402C:
      m_model	      = PS6402C;
      m_firstRange    = PS6000_50MV;
      m_lastRange     = PS6000_20V;
      m_channelCount  = 4;
      m_AWG           = false;
      m_awgBufferSize = 0;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6402D:
      m_model		= PS6402D;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_awgBufferSize = PS640X_C_D_MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6403:
      m_model		= PS6403;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_awgBufferSize = MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6403A:
      m_model		= PS6403;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = false;
      m_awgBufferSize = 0;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6403B:
      m_model		= PS6403B;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_AWG = MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6403C:
      m_model		= PS6403C;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = false;
      m_awgBufferSize = 0;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6403D:
      m_model		= PS6403D;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_awgBufferSize = PS640X_C_D_MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6404:
      m_model	      = PS6404;
      m_firstRange    = PS6000_50MV;
      m_lastRange     = PS6000_20V;
      m_channelCount  = 4;
      m_AWG           = true;
      m_awgBufferSize = MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6404A:
      m_model		= PS6404;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = false;
      m_awgBufferSize = 0;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6404B:
      m_model		= PS6404B;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_awgBufferSize = MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6404C:
      m_model		= PS6404C;
      m_firstRange = PS6000_50MV;
      m_lastRange = PS6000_20V;
      m_channelCount = 4;
      m_AWG = true;
      m_awgBufferSize = 0;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6404D:
      m_model         = PS6404D;
      m_firstRange    = PS6000_50MV;
      m_lastRange     = PS6000_20V;
      m_channelCount  = 4;
      m_AWG           = true;
      m_awgBufferSize = PS640X_C_D_MAX_SIG_GEN_BUFFER_SIZE;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++)
	{
	  m_channelSettings[i].range = PS6000_5V;
	  m_channelSettings[i].DCcoupled = PS6000_DC_1M;
	  m_channelSettings[i].enabled = true;
	}
      break;

    case PS6407:
      m_model	     = PS6407;
      m_firstRange   = PS6000_100MV;
      m_lastRange    = PS6000_100MV;
      m_channelCount = 4;
      m_AWG          = true;

      for (uint i = 0; i < PS6000_MAX_CHANNELS; i++) 
	{
	  m_channelSettings[i].range = PS6000_100MV;
	  m_channelSettings[i].DCcoupled = PS6000_DC_50R;
	  m_channelSettings[i].enabled = true;
	}
      break;

    default:
      break;
    }
}

void PS6000::initBuffer(unsigned short ch,bool init)
{
  PICO_STATUS status;

  if(init && m_buffers[ch]==nullptr)
    {
      m_buffers[ch]=new int16_t[PS6000_BUFFER_SIZE];

      status=ps6000SetDataBuffer(m_handle, (PS6000_CHANNEL)ch, &m_buffers[ch][0], PS6000_BUFFER_SIZE, PS6000_RATIO_MODE_NONE);
      if(status!=PICO_OK)
	throw std::string("Unable to set data buffer: "+status);
    }
  else if(!init && m_buffers[ch]!=nullptr)
    {
      delete[] m_buffers[ch];
      m_buffers[ch]=nullptr;
    }
}

void PS6000::setTrigger(uint8_t ch, float threshold, Direction direction)
{
  if(ch>=m_channelCount)
    throw std::string("Channel out of range"); //: "+ch+"/"+m_channelCount);

  float chfactor=((float)m_availRanges[m_channelSettings[ch].range])/PS6000_MAX_VALUE;
  
  PS6000_THRESHOLD_DIRECTION dir=PS6000_NONE;
  switch(direction)
    {
    case Rising:
      dir=PS6000_RISING;
      break;
    case Falling:
      dir=PS6000_FALLING;
      break;
    }

  ps6000SetSimpleTrigger(m_handle, true, (PS6000_CHANNEL)ch, threshold/chfactor, dir, 0, 0);
}
 
void PS6000::unsetTrigger(uint8_t ch)
{
  ps6000SetSimpleTrigger(m_handle, false, (PS6000_CHANNEL)ch, 0, PS6000_NONE, 0, 0);
}

void PS6000::setTriggerPosition(float percentage)
{
  m_triggerPosition=percentage;
}

float PS6000::getTriggerPosition()
{
  return m_triggerPosition;
}

void PS6000::setEnable(unsigned short ch, bool enable)
{
  if(ch>=m_channelCount)
    throw std::string("Channel out of range"); //: "+ch+"/"+m_channelCount);

  m_channelSettings[ch].enabled=enable;
  initBuffer(ch, enable);
}

bool PS6000::getEnable(unsigned short ch) const
{
  if(ch>=m_channelCount)
    throw std::string("Channel out of range"); //: "+ch+"/"+m_channelCount);

  return m_channelSettings[ch].enabled;
}

void PS6000::setRange(unsigned short ch, unsigned int range)
{
  if(ch>=m_channelCount)
    throw std::string("Channel out of range"); //: "+ch+"/"+m_channelCount);

  int16_t thisRangeIdx=PS6000_MAX_RANGES;
  for(int16_t rangeIdx=0;rangeIdx<PS6000_MAX_RANGES;rangeIdx++)
    if(m_availRanges[rangeIdx]==range)
      thisRangeIdx=rangeIdx;

  if(thisRangeIdx==PS6000_MAX_RANGES)
    throw std::string("Invalid range");

  m_channelSettings[ch].range=thisRangeIdx;
}

unsigned int PS6000::getRange(unsigned short ch) const
{
  if(ch>=m_channelCount)
    throw std::string("Channel out of range"); //: "+ch+"/"+m_channelCount);

  return m_availRanges[m_channelSettings[ch].range];
}

void PS6000::setPeriod(float period)
{
  if(period<=3.2e-9)
    {
      m_timebase=logf(period*5000000000)/logf(2);
      m_period=pow(2,m_timebase)/5000000000;
    }
  else
    {
      m_timebase=period*156250000+4;
      m_period=((float)(m_timebase-4))/156250000;
    } 
}

float PS6000::getPeriod() const
{ return m_period; }

void PS6000::configChannels()
{
  ps6000SetEts(m_handle, PS6000_ETS_OFF, 0, 0, 0); // Turn off ETS

  for(int16_t i = 0; i < m_channelCount; i++) // reset channels to most recent settings
    {
      ps6000SetChannel(m_handle, (PS6000_CHANNEL) (PS6000_CHANNEL_A + i),
		       m_channelSettings[PS6000_CHANNEL_A + i].enabled,
		       (PS6000_COUPLING)m_channelSettings[PS6000_CHANNEL_A + i].DCcoupled,
		       (PS6000_RANGE)m_channelSettings[PS6000_CHANNEL_A + i].range, 0, PS6000_BW_FULL);

      initBuffer(i, m_channelSettings[PS6000_CHANNEL_A + i].enabled);
    }
}

void PS6000::setSamples(uint32_t samples)
{ 
  if(samples>PS6000_BUFFER_SIZE)
    throw std::string("Requested number of samples too big");
  m_samples=samples; 
}

uint32_t PS6000::getSamples()
{ return m_samples; }

void PS6000::run(std::vector<std::vector<float>> &results)
{
  // figure out sampling rate
  uint32_t realSampleCount=m_samples;

  ps6000GetTimebase2(m_handle, m_timebase, m_samples, &m_period, 0, &realSampleCount, 0);
  m_period*=1e-9;

  // Gather data
  uint32_t pretriggerSamples=m_samples*m_triggerPosition;
  ps6000RunBlock(m_handle, pretriggerSamples, m_samples-pretriggerSamples, m_timebase, 1, nullptr, 0, nullptr, nullptr);

  int16_t ready=0;
  do
    {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      ps6000IsReady(m_handle, &ready);
    } 
  while(ready==0);

  ps6000GetValues(m_handle, 0, &realSampleCount, 1, PS6000_RATIO_MODE_NONE, 0, nullptr);

  //
  // Store the data
  for(int16_t ch = 0, i=0; ch < m_channelCount; ch++)
    {
      if(m_channelSettings[ch].enabled)
   	{
	  float chfactor=((float)m_availRanges[m_channelSettings[ch].range])/PS6000_MAX_VALUE;
  	  for(uint j=0;j<realSampleCount;j++)
	    results[i][j]=m_buffers[ch][j]*chfactor;
	  i++;
	}
    }

  //
  // Cleanup
  ps6000Stop(m_handle);
}

void PS6000::run(std::vector<std::vector<int16_t>> &results)
{
  // figure out sampling rate
  uint32_t realSampleCount=m_samples;

  ps6000GetTimebase2(m_handle, m_timebase, m_samples, &m_period, 0, &realSampleCount, 0);
  m_period*=1e-9;

  // Gather data
  uint32_t pretriggerSamples=m_samples*m_triggerPosition;
  ps6000RunBlock(m_handle, pretriggerSamples, m_samples-pretriggerSamples, m_timebase, 1, nullptr, 0, nullptr, nullptr);

  int16_t ready=0;
  do
    {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      ps6000IsReady(m_handle, &ready);
    } 
  while(ready==0);

  ps6000GetValues(m_handle, 0, &realSampleCount, 1, PS6000_RATIO_MODE_NONE, 0, nullptr);

  //
  // Store the data
  for(int16_t ch = 0, i=0; ch < m_channelCount; ch++)
    {
      if(m_channelSettings[ch].enabled)
   	{
  	  for(uint j=0;j<realSampleCount;j++)
	    results[i][j]=m_buffers[ch][j];
	  i++;
	}
    }

  //
  // Cleanup
  ps6000Stop(m_handle);
}

void PS6000::printInfo() const
{
  std::cout << "Picoscope 6000 Series" << std::endl;
  std::cout << " Model: " << std::hex << m_model << std::dec << std::endl;
  std::cout << " Channels: " << m_channelCount << std::endl;
}
