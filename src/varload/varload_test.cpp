#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "I2CFTDICom.h"
#endif

#include "Logger.h"
#include "SorensenPs.h"


loglevel_e loglevel = logINFO;

int main(int argc, char* argv[]) {
#ifndef FTDI
  logger(logERROR) << "FTDI support not enabled.";
  return -1;
#else 
  // Output log
  std::fstream logfile("varload.csv", std::fstream::out);
  logfile << "DACsetting current" << std::endl;

  // Initialize load power supply
  SorensenPs ps("/dev/ttyUSB2", 11);
  ps.init();
  ps.setVoltage(1.5);
  ps.setCurrent(3);

  // Initialize I2C (for DAC comm)
  std::shared_ptr<I2CCom> i2c=std::make_shared<I2CFTDICom>(0x48);

  // Turn on and scan currents
  ps.turnOn();

  uint16_t data;
  for(uint i=0;i<=0xAF;i++)
    {
      i2c->write_reg16(0x0, i);
      usleep(100);
      data=i2c->read_reg16(0x0);
      sleep(1);

      std::string curr=ps.getCurrent();
      std::cout << "0x" << std::hex << (uint)data << std::dec << " " << curr << std::endl;
      logfile << "0x" << std::hex << (uint)data << std::dec << " " << curr << std::endl;
    }

  ps.turnOff();
#endif

  

  return 0;
}
