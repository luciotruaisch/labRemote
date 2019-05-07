#include <iostream>
#include <fstream>
#include <chrono>

#include "Logger.h"

#ifdef SCOPE
#include "PS6000.h"
#endif

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[])
{
  if(argc!=2)
    {
      std::cerr << "usage: " << argv[0] << " outfile.csv" << std::endl;
      return 1;
    }

#ifndef SCOPE
  logger(logERROR) << "Missing libScope";
#else // SCOPE

  PicoScope *pico=new PS6000();
  pico->open();

  pico->setEnable(3,false);
  pico->setPeriod(1e-9);
  pico->setRange(0, 1000);
  pico->setRange(1,  100);
  pico->setRange(2, 1000);
  pico->configChannels();

  pico->setSamples(2048);
  pico->setTriggerPosition(0.5);

  pico->setTrigger(0, 500, PicoScope::Direction::Rising);

  float period=pico->getPeriod();

  std::vector<std::vector<float>> data(3,std::vector<float>(2048));

  std::ofstream logfile(argv[1], std::fstream::out);
  logfile << "run time A B C" << std::endl;

  uint32_t i=0;
  while(true)
    {
      auto start = std::chrono::steady_clock::now();

      pico->run(data);

      for(unsigned int row=0; row<data[0].size(); row++)
      {
  	logfile << i << " " << row*period;
  	for(unsigned int ch=0; ch<data.size(); ch++)
  	  logfile << " " << data[ch][row];
  	logfile << std::endl;
      }

      auto duration_read = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
      std::cout << "time after read: " << duration_read.count() << " ms" << std::endl;
      i++;
    }
  logfile.close();

  pico->close();

#endif // SCOPE

  return 0;
}
