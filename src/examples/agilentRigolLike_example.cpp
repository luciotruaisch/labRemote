#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Logger.h"
#include "AgilentPs.h"

//------ SETTINGS
loglevel_e loglevel = logINFO;
std::string port("/dev/ttyUSB0");
int gpib = 3;
int channel = 1;
//---------------


void usage(char *argv[]) {
  std::cerr << "Usage: " << argv[0] << " [options] command [parameters]" << std::endl;
  std::cerr << "List of possible COMMAND:" << std::endl;
  std::cerr << "  get-current    Get reading of current [A]" << std::endl;
  std::cerr << "  get-voltage    Get reading of voltage [V]" << std::endl;
  std::cerr << "  power-on [V I]   Power ON PS, optionally setting voltage to V in Volts and current to I in Ampere." << std::endl;
  std::cerr << "  power-off      Power OFF PS" << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "List of options:" << std::endl;
  std::cerr << " -c, --channel Ch  Set PS channel to work on to Ch (default: " << channel << ")" << std::endl;
  std::cerr << " -p, --port        Set serial port to which the PS is connected to (default: " << port << ")" << std::endl;
  std::cerr << " -g, --gpib        Set GPIB adress to which the PS is connected to (default: " << port << ")" << std::endl;
  std::cerr << " --debug           Enable more verbose printout"  << std::endl;
  std::cerr << "" << std::endl;
  std::cerr << "" << std::endl;
}

int main(int argc, char*argv[]) {

  if (argc < 1) {
    usage(argv);
    return 1;
  }

  //Parse command-line
  int c;
  const int DEBUG_OPTION = 1000;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
      {"channel",  required_argument, 0,  'c' },
      {"port",     required_argument, 0,  'p' },
      {"gpib",     required_argument, 0,  'g' },
      {"debug",    required_argument, 0,  DEBUG_OPTION },
      {0,          0,                 0,  0 }
    };
    
    c = getopt_long(argc, argv, "c:p:g:", long_options, &option_index);
    if (c == -1)
      break;

    switch (c) {
    case 'c':
      channel = atoi(optarg);
      break;
    case 'p':
      port = optarg;
      break;
    case 'g':
      gpib = atoi(optarg);
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

  std::string command;
  std::vector<std::string> params;
  if (optind < argc) {
    command = argv[optind++];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);
    while (optind < argc) {
      std::string p(argv[optind++]);
      std::transform(p.begin(), p.end(), p.begin(), ::tolower);
      params.push_back(p);
    }
  } else {
    std::cerr << "Required command argument missing." << std::endl;
    std::cerr << std::endl;
    usage(argv);
    return 1;
  }

  if (loglevel >= logDEBUG) {
    std::cout << "Settings: " << std::endl;
    std::cout << " USB port: " << port << std::endl;
    std::cout << " PS channel: " << channel << std::endl;
    std::cout << " GPIB adress: " << gpib << std::endl;
  }

  //Initialize PS communication
  AgilentPs PS(port, gpib);  

  //Now interpret command
  if (command == "get-current") {
    if (loglevel >= logDEBUG) std::cout << "Current: ";
    PS.setCh(channel);
    std::cout << PS.getCurrent();
    if (loglevel >= logDEBUG) std::cout << " A";
    std::cout << std::endl;
  } else if (command == "get-voltage") {
    if (loglevel >= logDEBUG) std::cout << "Voltage: ";
    PS.setCh(channel);
    std::cout << PS.getVoltage();
    if (loglevel >= logDEBUG) std::cout << " V";
    std::cout << std::endl;
  } else if (command == "power-off") {
    if (loglevel >= logDEBUG) std::cout << "Powering OFF." << std::endl;
    PS.turnOff();
  } else if (command == "power-on") {    
    if (loglevel >= logDEBUG) std::cout << "Powering ON." << std::endl;
    //PS.init();
    if (params.size() > 0) {
      if (params.size() != 2) {
	std::cerr << "Invalid number of parameters to power-on command." << std::endl;
	std::cerr << std::endl;
	usage(argv);
	return 1;
      }
      PS.setCh(channel);
      PS.setRange(8);// 8 or 35 for low voltage range, 20 or 60 for high voltage range
      PS.setVoltage(atof(params[0].c_str()));
      PS.setCurrent(atof(params[1].c_str()));
    }
    PS.turnOn();
  }

  return 0;
}
