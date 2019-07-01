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
  // Get settings from the command line
  if (argc != 11)
    {
      logger(logERROR) << "Not enough parameters!";
      logger(logERROR) << "Usage: " << argv[0] << " TESTNAME PB1 PB2 PB3 PB4 PB5 PB6 PB7 PB8 PB9";
      return -1;
    }

  std::string TestName = argv[1];
  std::vector<std::string> PBNames;
  for(uint i=0;i<9;i++)
    PBNames.push_back(argv[2+i]);

  //
  // Create log directory if it does not exist
  DIR* logdir=opendir("log");
  if(!logdir)
    {
      const int dir_err = mkdir("log", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
      if (-1 == dir_err)
	{
	  std::cerr << "Error creating log directory!" << std::endl;
	  return 1;
	}
    }
  else
    closedir(logdir);

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

  for(uint pbidx=2; pbidx<3; pbidx++)
    {
      PBv2Test test(TestName+"_"+PBNames[pbidx], tb, pbidx, ps);

      std::cout << ": " << tb->getP5VCurrent() << std::endl;
      std::cout << "Input LV Current (TB): " << tb->getVinCurrent() << std::endl;
      std::cout << "Input LV Voltage (TB): " << tb->getVin() << std::endl;

      tb->getPB(pbidx)->init();

      //test.runGeneral();
      //test.runLVEnable();
      test.runHVEnable(29.66);
      //test.runDCDCEfficiency();
      //test.runVin();
      //test.runVinIn();
    }
  

  // Power-off
  ps->turnOff();

  return 0;
}




