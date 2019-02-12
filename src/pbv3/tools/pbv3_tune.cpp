#include <unistd.h>
#include <string.h>
#include <getopt.h>

#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <json.hpp>
#include <fstream>

#include "Logger.h"

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"
#include "AMACv2.h"

#include "Bk85xx.h"
#include "AgilentPs.h"
#include "TTITSX1820PPs.h"
#include "TTIMX180TPPs.h"
#include "Keithley24XX.h"

#include "PBv3ConfigTools.h"

//------ SETTINGS
loglevel_e loglevel = logINFO;
std::string port("/dev/ttyUSB0");
int gpib = 10;
std::string outfile = "config.json";
bool outappend=false;
//---------------

void usage(char* argv[])
{
  std::cerr << "Usage: " << argv[0] << " [options] command [command...]" << std::endl;
  std::cerr << "List of possible COMMAND:" << std::endl;
  std::cerr << "  vddbg                            Tune the VDD bandgap." << std::endl;
  std::cerr << "  ambg                             Tune the AM bandgap." << std::endl;
  std::cerr << "  rampgain                         Tune the ADC slope." << std::endl;
  std::cerr << "  slope                            Recalibrate the ADC slope." << std::endl;
  std::cerr << "  ntc                              Recalibrate the NTCs." << std::endl;
  std::cerr << "  cur1v                            Tune the Cur1V CM block." << std::endl;
  std::cerr << "  cur10v                           Tune the Cur10V CM block." << std::endl;
  std::cerr << "  all                              Tune everything." << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "List of options:" << std::endl;
  std::cerr << " -p, --port        Set serial port to which the PS is connected to (default: " << port << ")" << std::endl;
  std::cerr << " -g, --gpib        Set GPIB adress to which the PS is connected to (default: " << gpib << ")" << std::endl;
  std::cerr << " -o, --output      File where to save the tuned configuration. (default: stdout)" << std::endl;
  std::cerr << " -a, --append      Modify existing output file instead of rewriting." << std::endl;
  std::cerr << " --debug           Enable more verbose printout"  << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "" << std::endl;

}

int main(int argc, char* argv[]) 
{
  if (argc < 1)
    {
      usage(argv);
      return 1;
    }

  int c;
  const int DEBUG_OPTION = 1000;
  while (1)
    {
      int option_index = 0;
      static struct option long_options[] =
	{
	  {"port",     required_argument, 0,  'p' },
	  {"gpib",     required_argument, 0,  'g' },
	  {"output",   required_argument, 0,  'o' },
	  {"append",   no_argument     , 0,  'a' },
	  {"debug",    no_argument     , 0,  DEBUG_OPTION },
	  {0,          0,              0,  0 }
	};

      c = getopt_long(argc, argv, "p:g:o:a", long_options, &option_index);
      if (c == -1)
	break;

      switch (c)
	{
	case 'p':
	  port = optarg;
	  break;
	case 'g':
	  gpib = atoi(optarg);
	  break;
	case 'o':
	  outfile = optarg;
	  break;
	case 'a':
	  outappend = true;
	  break;
	case DEBUG_OPTION:
	  loglevel = logDEBUG;
	  break;
	default:
	  std::cerr << "Invalid option supplied. Aborting." << std::endl;
	  std::cerr << std::endl;
	  usage(argv);
	}
    }

  std::vector<std::string> commands;
  if (optind < argc)
    {
      commands.push_back(argv[optind++]);
    }
  else
    {
      std::cerr << "Required command argument missing." << std::endl;
      std::cerr << std::endl;
      usage(argv);
      return 1;
    }

  logger(logDEBUG) << "Settings";
  logger(logDEBUG) << " USB port: " << port;
  logger(logDEBUG) << " GPIB adress: " << gpib;

  //
  // Open configuration
  json config;
  if(outappend)
    {
      std::ifstream fh_in(outfile);
      if(fh_in.is_open())
	fh_in >> config;
    }


  //
  // Initialize PS and communication                                                                                                                                               

  // Prog ID set during init (require power-up to be set)
  unsigned short int amacid=0x0;

  // Init Agilent
  logger(logINFO) << "Init Agilent PS";
  AgilentPs ps(port, gpib);
  //logger(logINFO) << "Init TTITSX PS";
  //TTIMX180TPPs ps(agiDev, 10);
  //TTITSX1820PPs ps(agiDev, 10);
  try
    {
      ps.init();
      ps.setCh(1);
      ps.setRange(20);
      ps.setVoltage(11.0);
      ps.setCurrent(2.00);
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }
    
  // Turn on power
  logger(logINFO) << "Turn on PS fully";
  ps.setVoltage(11.0);
  ps.setCurrent(2.00);
  ps.turnOn();
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  // Init com
  logger(logINFO) << "Init AMAC";
  std::shared_ptr<AMACv2> amac;
  try {
    amac.reset(new AMACv2(amacid, std::unique_ptr<EndeavourRaw>(new EndeavourRawFTDI())));
    amac->init();
    PBv3ConfigTools::configAMAC(amac, config, false);
    amac->initRegisters();
  } catch(EndeavourComException &e) {
    logger(logERROR) << "Unable to initialize AMACv2";
    logger(logERROR) << e.what();
    return 1;
  }

  //
  // Run over all commands
  for(const std::string& command : commands)
    {
      logger(logINFO) << "Running " << command;
      if(command=="vddbg")
	config.merge_patch(PBv3ConfigTools::tuneVDDBG      (amac));
      if(command=="ambg")
	config.merge_patch(PBv3ConfigTools::tuneAMBG       (amac));
      if(command=="rampgain")
	config.merge_patch(PBv3ConfigTools::tuneRampGain   (amac));
      if(command=="slope")
	config.merge_patch(PBv3ConfigTools::calibrateSlope (amac));
      if(command=="offset")
	config.merge_patch(PBv3ConfigTools::calibrateOffset(amac));
      if(command=="ntc")
	config.merge_patch(PBv3ConfigTools::calibrateNTC   (amac));
      if(command=="cur10v")
	config.merge_patch(PBv3ConfigTools::tuneCur10V     (amac));
      if(command=="cur1v")
	config.merge_patch(PBv3ConfigTools::tuneCur1V      (amac));
      if(command=="all")
	{
	  config.merge_patch(PBv3ConfigTools::calibrateSlope (amac));
	  config.merge_patch(PBv3ConfigTools::calibrateOffset(amac));
	  config.merge_patch(PBv3ConfigTools::tuneVDDBG      (amac));
	  config.merge_patch(PBv3ConfigTools::tuneAMBG       (amac));
	  config.merge_patch(PBv3ConfigTools::tuneRampGain   (amac));
	  config.merge_patch(PBv3ConfigTools::tuneCur10V     (amac));
	  config.merge_patch(PBv3ConfigTools::tuneCur1V      (amac));
	  config.merge_patch(PBv3ConfigTools::calibrateNTC   (amac));
	}
    }

  //
  // Save configuration
  std::ofstream fh_out(outfile);
  fh_out << std::setw(4) << config << std::endl;
  fh_out.close();

  //
  // Turn off power
  logger(logINFO) << "Power off";
  ps.turnOff();

  return 0;
}
