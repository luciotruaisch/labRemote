#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "FTDICom.h"
#endif

#include "Logger.h"
#include "SorensenPs.h"


loglevel_e loglevel = logINFO;

int main(int argc, char* argv[]) {
#ifndef FTDI
  log(logERROR) << "FTDI support not enabled.";
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
  std::unique_ptr<FTDICom> i2c(new FTDICom());
  i2c->enableI2C();

  // Turn on and scan currents
  ps.turnOn();

 char data[2];
  for(uint i=0;i<=0xAF;i++)
    {
      data[0]=i;
      data[1]=0x0;
      i2c->writeI2C(0x48, 0x0, data, 2);
      usleep(100);
      data[0]=0;
      data[1]=0;
      i2c->readI2C(0x48, 0x0, data, 2);
      sleep(1);

      std::string curr=ps.getCurrent();
      std::cout << "0x" << std::hex << (uint)data[0] << std::dec << " " << curr << std::endl;
      logfile << "0x" << std::hex << (uint)data[0] << std::dec << " " << curr << std::endl;
    }

  ps.turnOff();
#endif

  

  return 0;
}
