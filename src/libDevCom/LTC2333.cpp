#include "LTC2333.h"
#include <unistd.h>
LTC2333::LTC2333(std::shared_ptr<DeviceCom> dev) : m_dev(dev){
  init();
}

LTC2333::~LTC2333()
{ 

}

void LTC2333::init(){
  reset();
}

void LTC2333::reset(){

  // NOTE: we have seen on initial usage of the SPI bus that we sometimes have
  // 24 clock edges instead of the expected 8 clock edges. If this persists, we should
  // call setADC(vec) on each reset, and potentially similarly for the DAC and 
  // potentiometer code (if deemed to be needed / to be affecting anything).
  // We expect that this just sends garbage which is never used, however.

}

bool LTC2333::checkValidInput(unsigned int chan, unsigned int span){

  if(chan > m_chanMax){
    std::cout << "LTC2333::checkValidInput ERROR chan is" << std::hex << chan << ", but chanMax is" << std::hex << m_chanMax << std::endl;
    std::cout << "Exiting checkValidInput" << std::endl;
    return false;
  }

  if(span > m_spanMax){
    std::cout << "LTC2333::checkValidInput ERROR span is" << std::hex << span << ", but spanMax is" << std::hex << m_spanMax << std::endl;
    std::cout << "Exiting checkValidInput" << std::endl;
    return false;
  }

  return true;
}


unsigned int LTC2333::formatCommand(unsigned int chan, unsigned int span){

  // cmd header for MUX control (10 in binary)
  unsigned int cmd = 0x2;

  chan = chan & m_chanMax;
  span = span & m_spanMax;

  // input is 8 bits, so shift the 2-bit header over by 6
  // and the 3-bit middle part by 3
  unsigned int input = (cmd << 6) | (chan << 3) | span;
  return input;
}

std::vector<unsigned int> LTC2333::formatConversionResult(uint8_t* data){

  // Here we parse the 8-bit data into a single unsigned int
  unsigned int output = 0;
  for(unsigned int index = 0; index < m_nBytesData; ++index){
    output = (output << 8) | data[index];
  }

  // Now split output into its components
  unsigned int output_span = output & m_spanMax;
  unsigned int output_chan = (output >> 3) & m_chanMax;

  // Here is the 18-bit result
  unsigned int output_result = (output >> 6) & 0xFFFF; // 0xFFFF = 2^16-1

  return {output_result, output_chan, output_span};
}


void LTC2333::setADC(std::vector<std::pair<uint8_t,uint8_t>> inputSettings){
  uint8_t nBytesIn = inputSettings.size();
  if(nBytesIn > 16){
    std::cout << "LTC2333::setADC ERROR too many chan/span" << std::endl;
    std::cout << "Exiting setADC" << std::endl;
    return;
  }
  //std::cout << "nbytesIn: " << unsigned(nBytesIn) << std::endl;
  uint8_t tx[nBytesIn] = {0, };

  for(unsigned int i = 0; i < nBytesIn; ++i){

    auto inputPair = inputSettings.at(i);

    uint8_t chan = inputPair.first;
    uint8_t span = inputPair.second;

    // Check that chan and span are in range
    if( !checkValidInput(chan, span) ){
      // ERROR and maybe return
      std::cout << "LTC2333::setADC ERROR invalid chan/span" << std::endl;
      std::cout << "Exiting setADC" << std::endl;
      return;
    }

    // Format our 8-bit word of cmd code, chan, and span
    tx[i] = (uint8_t) formatCommand(chan, span);
    //std::cout << "tx[" << i << "] is " << std::hex << unsigned(tx[i]) << std::endl;
  }

  m_inputSettings = inputSettings;
  m_nBytesIn = nBytesIn;
  m_dev->read_reg(tx, NULL, nBytesIn);
  m_init = true;
  usleep(m_nBytesIn); //sleep for tx
}

std::vector<LTC2333Outputs> LTC2333::getADC(){

  if(!m_init){
    std::cout << "LTC2333::getADC ERROR no previously initiated conversion to read!" << std::endl;
    std::cout << "Exiting getADC" << std::endl;
    return {};
  }

  //Set ADC_CNV pin FIXME remove hardcoding
  uint32_t data = m_dio->read_reg(0x2); 
  uint32_t mask = (uint32_t)(1 << 7);
  data = (data & ~mask) | (true << 7);
  m_dio->write_reg(0x2, data);
  //Reset ADC_CNV pin
  data = m_dio->read_reg(0x2); 
  mask = (uint32_t)(1 << 7);
  data = (data & ~mask) | (false << 7);
  m_dio->write_reg(0x2, data);
  
  usleep(1); //wait for conversion to finish
  uint8_t rx[3*m_nBytesIn] = {0, }; 
  uint8_t  dummy [3*m_nBytesIn] = {0, };
  m_dev->read_reg(dummy, NULL, 3*m_nBytesIn); //activate CS+clock for a 192 clock cycles
  usleep(m_nBytesIn); //wait for rx buffer to fill
  m_dev->read_reg(dummy, rx, 3*m_nBytesIn); //recover data in rx (clocks will also be activated...)
  std::vector<LTC2333Outputs> outputs;

  // Iterate 3-bytes at a time
  for(unsigned int i = 0; i < 3*m_nBytesIn; i+=3){
    LTC2333Outputs output_i;
    // 16 bit conversion result
    output_i.result = (rx[i] << 8) | (rx[i+1] );
    output_i.chan = (rx[i+2] >> 3) & m_chanMax;
    output_i.span = rx[i+2] & m_spanMax;
    outputs.push_back(output_i);
  }
  return outputs;
}


LTC2333Outputs LTC2333::setAndReadChan(uint8_t chan, uint8_t span){

  std::vector<std::pair<uint8_t,uint8_t>> vec;
  vec.push_back( std::make_pair(chan, span) );
  // initializes the conversion
  setADC(vec);
  //usleep(100); //for good measure
  return getADC().at(0);
}