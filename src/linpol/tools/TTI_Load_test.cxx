#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include <fstream>

#include "Logger.h"

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"


#include "Bk85xx.h"
#include "AgilentPs.h"
#include "TTILD400P.h"
#include "TTIMX180TPPs.h"

#include "LinPOL12VTestTools.h"

void usage(char* argv[])
{
  std::cerr << "Usage: " << argv[0] << " datadir GPIB" << std::endl;
}


int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      usage(argv);
      return 1;
    }



  if (argc-optind < 1)
    {
      std::cout << argc << " " << optind << " " << 1 << std::endl;
      std::cerr << "Required paths missing." << std::endl;
      std::cerr << std::endl;
      usage(argv);
      return 1;
    }

  std::string outDir = argv[optind++]; 
  std::string agiDev = argv[optind++];
  
  logger(logDEBUG) << "Settings";
 
  logger(logDEBUG) << " agiDev: " << agiDev;

  TTILD400P load2(agiDev,25);
  
  try
    {
      load2.init();
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  std::string mode;
  try
    {
      std::cout << "set mode" << std::endl;
      //load2.setModeCC();
      std::cout << "get mode" << std::endl;
      mode = load2.getMode();  
      std::cout << "done" << std::endl;
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 2;
    }

  std::cout<<"Current Mode: "<<mode<<std::endl;  
  
}
