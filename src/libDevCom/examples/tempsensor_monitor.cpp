#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>
#include <chrono>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "I2CFTDICom.h"
#endif

#include "HIH6130.h"

#define NUMCHAR(X) (uint)((unsigned char)(X))

int main(int argc, char* argv[]) {
#ifndef FTDI
  std::cerr << "FTDI support not enabled." << std::endl;
  return -1;
#else 
  // Initialize I2C (for DAC comm)
  std::shared_ptr<I2CCom> i2c(new I2CFTDICom(0x27));

  HIH6130 tempsensor(i2c);

  // Loop and measure
  while(true)
    {
      // Perform the measurement
      std::time_t time = std::time(nullptr);

      tempsensor.read();

      // Save the data
      std::cout << time << "\t" << tempsensor.status() << "\t" << tempsensor.humidity() << "\t" << tempsensor.temperature() << std::endl;
    }

#endif

  

  return 0;
}
