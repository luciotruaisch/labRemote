#include "PS6000.h"

#include <iostream>
#include <fstream>

int main()
{
  std::cout << "Hello world!" << std::endl;

  PicoScope *pico=new PS6000();
  pico->open();
  pico->printInfo();

  pico->setPeriod(1e-8);
  pico->setRange(0, 200);
  pico->configChannels();

  std::vector<std::vector<float>> data=pico->run();
  float period=pico->getPeriod();

  std::fstream fh;
  fh.open("data.csv", std::fstream::out);
  fh << "time dataA dataB dataC dataD" << std::endl;

  for(unsigned int row=0; row<data[0].size(); row++)
    {
      fh << row*period << " " << data[0][row] << " " << data[1][row] << " " << data[2][row] << " " << data[3][row] << std::endl;
    }

  fh.close();



  pico->close();
}
