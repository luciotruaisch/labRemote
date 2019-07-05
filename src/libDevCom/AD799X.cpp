#include "AD799X.h"

#include "LinearCalibration.h"

#include "NotSupportedException.h"


const std::map<AD799X::Model, AD799XModelInfo> AD799X::ModelInfo =
  {
   {AD799X::Model::AD7993, AD799XModelInfo({.NumChannels=4, .MaxValue=0x3FF}) },
   {AD799X::Model::AD7994, AD799XModelInfo({.NumChannels=4, .MaxValue=0xFFF}) },
   {AD799X::Model::AD7997, AD799XModelInfo({.NumChannels=8, .MaxValue=0x3FF}) },
   {AD799X::Model::AD7998, AD799XModelInfo({.NumChannels=8, .MaxValue=0xFFF}) }
  };

AD799X::AD799X(double reference, Model model, std::shared_ptr<I2CCom> com)
  : ADCDevice(std::make_shared<LinearCalibration>(reference, ModelInfo.at(model).MaxValue)),
    m_model(model), m_numChannels(ModelInfo.at(model).NumChannels), m_com(com)
{ }

AD799X::~AD799X()
{ }

uint32_t AD799X::readCount()
{
  std::vector<uint8_t> data(2);
  m_com->read_block(0x0, data);

  uint16_t chresult=(data[0]<<8)|(data[1]<<0);
  // TODO: Check if returned channel is the correct one
  //uint8_t  ch      =((chresult>>12)&0x007);
  uint32_t chcounts=(m_model==AD7993 || m_model==AD7997)?((chresult>> 2)&0x3FF):(chresult&0xFFF);
  return chcounts;
}

uint32_t AD799X::readCount(uint8_t ch)
{
  uint8_t chmask=0;

  switch(m_model)
    {
    case AD7993:
    case AD7994:
      chmask=(1<<ch);
      break;
    case AD7997:
    case AD7998:
      chmask=0x8|ch;
      break;
    default:
      throw NotSupportedException("Model not supported by AD799X");
      break;
    };

  std::vector<uint8_t> data(2);
  m_com->read_block((chmask<<4)&0xFF, data);
  uint16_t chresult=(data[0]<<8)|(data[1]<<0);
  // TODO: Check if returned channel is the correct one
  //uint8_t  ch      =((chresult>>12)&0x007);
  uint32_t chcounts=(m_model==AD7993 || m_model==AD7997)?((chresult>> 2)&0x3FF):(chresult&0xFFF);
  return chcounts;
}

void AD799X::readCount(const std::vector<uint8_t>& chs, std::vector<uint32_t>& counts)
{
  uint8_t chmask=0;
  for(uint8_t ch : chs)
    chmask|=(1<<ch);

  std::vector<uint8_t> data(2*chs.size());

  switch(m_model)
    {
    case AD7993:
    case AD7994:
      m_com->read_block((chmask<<4)&0xFF, data);
      break;
    case AD7997:
    case AD7998:
      m_com->write_reg16(0x2, (chmask<<4)|8);
      m_com->read_block(0x70, data);
      break;
    default:
      throw NotSupportedException("Model not supported by AD799X");
      break;
    };

  counts.clear();
  for(uint8_t chi=0;chi<chs.size();chi++)
    {
      uint16_t chresult=(data[2*chi+0]<<8)|(data[2*chi+1]<<0);
      // TODO: Check if returned channel is the correct one
      //uint8_t  ch      =((chresult>>12)&0x007);
      uint32_t chcounts=(m_model==AD7993 || m_model==AD7997)?((chresult>> 2)&0x3FF):(chresult&0xFFF);
      counts.push_back(chcounts);
    }
}
