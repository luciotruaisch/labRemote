#include <iostream>
#include <fstream>
#include <chrono>

#include "Logger.h"

#ifdef SCOPE
#include "PS6000.h"
#endif

loglevel_e loglevel = logINFO;

#define BUFFER_SIZE 2097152

int main()
{
#ifndef SCOPE
  log(logERROR) << "Missing libScope";
#else // SCOPE

  PicoScope *pico=new PS6000();
  pico->open();

  pico->setEnable(1,false);
  pico->setEnable(2,false);
  pico->setEnable(3,false);
  pico->setPeriod(1e-8);
  pico->setRange(0, 2000);
  pico->configChannels();

  float period=pico->getPeriod();

  std::vector<std::vector<std::vector<float>>> allrunsdata(10, std::vector<std::vector<float>>(1,std::vector<float>(BUFFER_SIZE)));
  //std::vector<std::vector<std::vector<int16_t>>> allrunsdata(10, std::vector<std::vector<int16_t>>(1,std::vector<int16_t>(BUFFER_SIZE)));

  std::ofstream logfile("/tmp/scope.csv", std::fstream::out);
  logfile << "run time A" << std::endl;

  for(uint i=0; i<10; i++)
    {
      auto start = std::chrono::steady_clock::now();

      pico->run(allrunsdata.at(i));

      auto duration_read = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
      std::cout << "time after read: " << duration_read.count() << " ms" << std::endl;
    }

  auto start = std::chrono::steady_clock::now();

  for(uint i=0;i<allrunsdata.size();i++)
    for(unsigned int row=0; row<allrunsdata[i][0].size(); row++)
      logfile << i << " " << row*period << " " << allrunsdata[i][0][row] << std::endl;

  auto duration_write = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
  std::cout << "time after write: " << duration_write.count() << " ms" << std::endl;
  logfile.close();

  pico->close();

#endif // SCOPE

  return 0;
}
