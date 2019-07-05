#include <memory>
#include <iostream>
#include <cmath>
#include <string>

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
  if (argc != 20)
    {
      logger(logERROR) << "Not enough parameters!";
      logger(logERROR) << "Usage: " << argv[0] << " TESTNAME PB1 K09 PB2 K10 PB3 L01 PB4 L02 PB5 L03 PB6 L06 PB7 L05 PB8 K11 PB9 K12";
      return -1;
    }

  std::string TestName = argv[1];
  std::vector<std::string> PBNames;
  std::vector<std::string> AMACNames;
  for(uint i=0;i<9;i++)
    {
      PBNames.push_back(argv[2+i*2]);
      AMACNames.push_back(argv[3+i*2]);
    }
  

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

  //Initialize power supply
  std::shared_ptr<SorensenPs> ps=std::make_shared<SorensenPs>("/dev/ttyUSB0", 11);

  try
    {
      //Set LV power supply
      ps->setCh(1);
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
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
 
  std::shared_ptr<PBv2TB> tb=std::make_shared<PBv2TB>();

  for(uint pbidx=0; pbidx<1; pbidx++)
    {

      PBv2Test test(TestName+"_"+PBNames[pbidx], tb, pbidx, ps);
      std::cout << ": " << tb->getP5VCurrent() << std::endl;
     

      std::cout << "Input LV Current (TB): " << tb->getVinCurrent() << std::endl;
      std::cout << "Input LV Voltage (TB): " << tb->getVin() << std::endl;

      tb->getPB(pbidx)->init();

      test.runGeneral();
      test.runLVEnable();
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      test.runHVEnable(30.0);
      ps->setCh(1);
      //test.runLeakage(AMACNames[pbidx]);
      //test.runDCDCEfficiency();
      //test.runVin();
      //test.runVinIn();
    }
  

  // Power-off
  ps->setCh(2);
  ps->turnOff();
  ps->setCh(1);
  ps->turnOff();


  return 0;
}




