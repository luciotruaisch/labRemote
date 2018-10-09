#include <memory>
#include <iostream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#include "Logger.h"
#include "SorensenPs.h"

#include "PBv2TB.h"
#include "PBv2Test.h"

loglevel_e loglevel = logINFO;
//make sure this program is modular - uses pbv2 test class and all of that
//separate different tests into methods, call all of them? 
int main(int argc, char* argv[])
{
  //
  // Run tests
  logger(logINFO) << "Initialising ...";
    
  logger(logINFO) << " ... Agilent PS:";
  std::shared_ptr<SorensenPs> ps=std::make_shared<SorensenPs>("/dev/ttyUSB0", 11);  
  try
    {
      ps->setCh(2);
      ps->init();
      ps->setVoltage(11);
      ps->setCurrent(5);
      ps->turnOn();
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  std::shared_ptr<PBv2TB> tb=std::make_shared<PBv2TB>();

  //
  // Power-off

  ps->turnOff();

  return 0;
}




