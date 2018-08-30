#ifndef AD799X_H
#define AD799X_H

#include <stdint.h>

#include <memory>
#include <unordered_map>

#include "I2CCom.h"
#include "ADCDevice.h"

struct AD799XModelInfo
{
  int32_t NumChannels;
  int32_t MaxValue;
};

class AD799X : public ADCDevice
{
public:
  enum Model {AD7993, AD7994, AD7997, AD7998};

  AD799X(float reference, Model model, std::shared_ptr<I2CCom> com);
  virtual ~AD799X();

  virtual uint32_t readCount();
  virtual uint32_t readCountChannel(uint8_t ch);
  virtual void readCountChannels(const std::vector<uint8_t>& chs, std::vector<uint32_t>& data);

private:
  // Model information
  static const std::unordered_map<Model, AD799XModelInfo> ModelInfo;
  
  // Properties of device
  Model m_model;
  uint32_t m_numChannels = 0;
  
  std::shared_ptr<I2CCom> m_com;
};

#endif // AD799X_H
