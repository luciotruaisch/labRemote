#include "PS6000.h"

#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include <math.h>

#define BUFFER_SIZE 8192

PS6000::PS6000()
  : PicoScope()
{ }

PS6000::~PS6000()
{ }

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
      m_model		= PS6402C;
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

void PS6000::configChannels()
{
  ps6000SetEts(m_handle, PS6000_ETS_OFF, 0, 0, 0); // Turn off ETS

  for(int16_t i = 0; i < m_channelCount; i++) // reset channels to most recent settings
    {
      ps6000SetChannel(m_handle, (PS6000_CHANNEL) (PS6000_CHANNEL_A + i),
		       m_channelSettings[PS6000_CHANNEL_A + i].enabled,
		       (PS6000_COUPLING)m_channelSettings[PS6000_CHANNEL_A + i].DCcoupled,
		       (PS6000_RANGE)m_channelSettings[PS6000_CHANNEL_A + i].range, 0, PS6000_BW_FULL);
    }
}

void PS6000::setPeriod(float period)
{
  if(period<=3.2e-9)
    {
      m_timebase=logf(period*5000000000)/logf(2);
      m_period=pow(2,m_timebase)/5000000000;
      std::cout << "m_timebase " << m_timebase << std::endl;
    }
  else
    {
      m_timebase=period*156250000+4;
      m_period=((float)(m_timebase-4))/156250000;
    } 
}

float PS6000::getPeriod()
{ return m_period; }

void PS6000::setRange(unsigned short ch, int range)
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

std::vector<std::vector<float>> PS6000::run()
{
  PICO_STATUS status;

  // Configure buffers for readback
  int16_t buffers[4][BUFFER_SIZE];

  for(int16_t i = 0; i < m_channelCount; i++)
    {
      if(m_channelSettings[i].enabled)
	{
	  status=ps6000SetDataBuffer(m_handle, (PS6000_CHANNEL)i, &buffers[i][0], BUFFER_SIZE, PS6000_RATIO_MODE_NONE);
	  if(status!=PICO_OK)
	    throw std::string("Unable to set data buffer: "+status);
	}
    }

  // figure out sampling rate
  uint32_t sampleCount=BUFFER_SIZE;
  uint32_t realSampleCount;
  ps6000GetTimebase2(m_handle, m_timebase, sampleCount, &m_period, 0, &realSampleCount, 0);
  m_period*=1e-9;
  std::cout << "Will sample " << realSampleCount << " / " << sampleCount << std::endl;

  // Gather data
  ps6000RunBlock(m_handle, 0, sampleCount, m_timebase, 1, nullptr, 0, nullptr, nullptr);

  int16_t ready=0;
  do
    {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      ps6000IsReady(m_handle, &ready);
    } 
  while(ready==0);

  ps6000GetValues(m_handle, 0, &sampleCount, 1, PS6000_RATIO_MODE_NONE, 0, nullptr);

  //
  // Store the data
  std::vector<std::vector<float>> result;
  for(int16_t ch = 0; ch < m_channelCount; ch++)
    {
      std::vector<float> chresult;
      if(m_channelSettings[ch].enabled)
	{
	  for(uint j=0;j<sampleCount;j++)
	    chresult.push_back((float)buffers[ch][j]*m_availRanges[m_channelSettings[ch].range]/PS6000_MAX_VALUE);
	}
      result.push_back(chresult);
    }

  //
  // Cleanup
  ps6000Stop(m_handle);

  return result;
}

void PS6000::printInfo() const
{
  std::cout << "Picoscope 6000 Series" << std::endl;
  std::cout << " Model: " << std::hex << m_model << std::dec << std::endl;
  std::cout << " Channels: " << m_channelCount << std::endl;
}
