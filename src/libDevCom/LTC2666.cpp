#include "LTC2666.h"

LTC2666::LTC2666(std::shared_ptr<DeviceCom> dev) : m_dev(dev){ 
  init();
}

LTC2666::~LTC2666()
{ 

}

void LTC2666::init(){
  reset();
}

void LTC2666::reset(){
  // originally did a power down and reset to DAC count = 0 here,
  // but doesn't make sense to do this anymore if we want to use these on the fly
  
}

void LTC2666::writeUpdateAll(unsigned int counts){
  
  // cmd header for write and update all DACs
  unsigned int cmd = 0xA0;
  unsigned int leadDigits = (counts >> 8) & 0xFF;
  unsigned int subleadDigits = counts & 0xFF;

  m_dev->write_reg({cmd, leadDigits, subleadDigits});

}
void LTC2666::writeUpdateChan(unsigned int chan, unsigned int counts){

  if(chan > m_chanMax){
    std::cout << "LTC2666::writeUpdateChan ERROR chan is" << std::hex << chan << ", but chanMax is" << std::hex << m_chanMax << std::endl;
    std::cout << "Exiting writeUpdateChan" << std::endl;
    return;
  }

  // cmd header for write and update channel
  unsigned int cmd = 0x30;

  // Add 0x30 + 0x0Y (for channel Y)
  cmd += chan;

  unsigned int leadDigits = (counts >> 8) & 0xFF;
  unsigned int subleadDigits = counts & 0xFF;

  m_dev->write_reg({cmd, leadDigits, subleadDigits});

}
void LTC2666::powerDownAll(){

  // cmd header to turn off all DACs.
  unsigned int cmd = 0x50;
  
  // Other values don't matter.
  m_dev->write_reg({cmd, 0x00, 0x00});

}
void LTC2666::powerDownChan(unsigned int chan){

  if(chan > m_chanMax){
    std::cout << "LTC2666::powerDownChan ERROR chan is" << std::hex << chan << ", but chanMax is" << std::hex << m_chanMax << std::endl;
    std::cout << "Exiting powerDownChan" << std::endl;
    return;
  }

  // cmd header for power down channel
  unsigned int cmd = 0x40;

  // Add 0x40 + 0x0Y (for channel Y)
  cmd += chan;

  // Other values don't matter.
  m_dev->write_reg({cmd, 0x00, 0x00});

}
void LTC2666::changeSpanAll(unsigned int span){

  if(span > m_spanMax){
    std::cout << "LTC2666::changeSpanAll ERROR span is" << std::hex << span << ", but spanMax is" << std::hex << m_spanMax << std::endl;
    std::cout << "Exiting changeSpanAll" << std::endl;
    return;
  }

  // cmd header for change span for all
  unsigned int cmd = 0xE0;

  // Middle value doesn't matter
  m_dev->write_reg({cmd, 0x00, span});

}
void LTC2666::changeSpanChan(unsigned int chan, unsigned int span){

  if(chan > m_chanMax){
    std::cout << "LTC2666::changeSpanChan ERROR chan is" << std::hex << chan << ", but chanMax is" << std::hex << m_chanMax << std::endl;
    std::cout << "Exiting changeSpanChan" << std::endl;
    return;
  }

  if(span > m_spanMax){
    std::cout << "LTC2666::changeSpanChan ERROR span is" << std::hex << span << ", but spanMax is" << std::hex << m_spanMax << std::endl;
    std::cout << "Exiting changeSpanChan" << std::endl;
    return;
  }

  // cmd header for change span for channel
  unsigned int cmd = 0x60;

  // Add 0x60 + 0x0Y (for channel Y)
  cmd += chan;

  // Middle value doesn't matter
  m_dev->write_reg({cmd, 0x00, span});

}
void LTC2666::pointMuxAtChan(unsigned int muxChan){

  if(muxChan > m_muxChanMax){
    std::cout << "LTC2666::pointMuxAtChan ERROR muxChan is" << std::hex << muxChan << ", but muxChanMax is" << std::hex << m_muxChanMax << std::endl;
    std::cout << "Exiting pointMuxAtChan" << std::endl;
    return;
  }

  // cmd header for MUX command
  unsigned int cmd = 0xB0;

  // to select particular channel
  unsigned int chanSel = 0x10;

  // Add 0x10 + 0x0Y (for mux chan Y)
  chanSel += muxChan;

  // Middle value doesn't matter
  m_dev->write_reg({cmd, 0x00, chanSel});

}
void LTC2666::disableMux(){

  // cmd header for MUX command
  unsigned int cmd = 0xB0;

  // to disable MUX
  unsigned int chanSel = 0x00;

  // Middle value doesn't matter
  m_dev->write_reg({cmd, 0x00, chanSel});

}
