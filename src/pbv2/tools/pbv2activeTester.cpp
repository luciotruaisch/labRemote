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
  if (argc < 3)
    {
      logger(logERROR) << "Not enough parameters!";
      logger(logERROR) << "Usage: " << argv[0] << " TESTNAME AMAC1";
      return -1;
    }

  std::string TestName = argv[1];
  std::string pbname = argv[2];

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
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::shared_ptr<PBv2TB> tb=std::make_shared<PBv2TB>();
  PBv2Test test(TestName+"_"+pbname, tb, 0, ps);

  test.runGeneral();
  test.runLVEnable();
  test.runDCDCEfficiency();
  test.runVin();

//   //
//   // Testing Current Sense Amp & Efficiency...
//   double vin_min = 4.0;
//   double vin_max = 5.0;
//   double vin_step = 0.05;


//   for(int j = 1; j<=9; j++){
//     std::string name;
//     switch (j)
//     {
//     case 1 : 
//       name = TestName + "_" + pb1;
//       break;
//     case 2 : 
//       name = TestName + "_" + pb2;
//       break;
//     case 3 : 
//       name = TestName + "_" + pb3;
//       break;
//     case 4 : 
//       name = TestName + "_" + pb4;
//       break;
//     case 5 : 
//       name = TestName + "_" + pb5;
//       break;
//     case 6 : 
//       name = TestName + "_" + pb6;
//       break;
//     case 7 : 
//       name = TestName + "_" + pb7;
//       break;
//     case 8 : 
//       name = TestName + "_" + pb8;
//       break;
//     default: 
//       name = TestName + "_" + pb9;
//     }

//     logger(logINFO) << "Testing current usage as a funciton of input voltage...";
//     //int DACNum = tb.getDAC_Num(j);
//     //int CHVALUE = tb.getDAC_CH(j);
//     logpath = "log/" + name + "_VinIin.log";
//     logfile.open(logpath, std::fstream::out);
//     logfile << "Vin Iin IinADC Vout" << std::endl;

//     for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
//       ps->turnOff();
//       ps->setVoltage(vin);
//       ps->turnOn();
//       amac1->write(AMACreg::LV_ENABLE, 0x1);
//       amac2->write(AMACreg::LV_ENABLE, 0x1);
//       amac3->write(AMACreg::LV_ENABLE, 0x1);
//       amac4->write(AMACreg::LV_ENABLE, 0x1);
//       amac5->write(AMACreg::LV_ENABLE, 0x1);
//       amac6->write(AMACreg::LV_ENABLE, 0x1);
//       amac7->write(AMACreg::LV_ENABLE, 0x1);
//       amac8->write(AMACreg::LV_ENABLE, 0x1);
//       amac9->write(AMACreg::LV_ENABLE, 0x1);

//       std::this_thread::sleep_for(std::chrono::seconds(2));

//       //double lv = dc.getValues().vol;//mV
//       //should this be DAC values or tb.getVinCurrent()?
//       double lvADC = tb.getVout(j); //get vout
//       std::cout << vin << "\t" << ps->getCurrent() << "\t" << tb.getVinCurrent() << "\t" << lvADC << std::endl;
//       logfile << vin << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << lvADC << std::endl;
//     }
//     amac1->write(AMACreg::LV_ENABLE, 0x0);
//     amac2->write(AMACreg::LV_ENABLE, 0x0);
//     amac3->write(AMACreg::LV_ENABLE, 0x0);
//     amac4->write(AMACreg::LV_ENABLE, 0x0);
//     amac5->write(AMACreg::LV_ENABLE, 0x0);
//     amac6->write(AMACreg::LV_ENABLE, 0x0);
//     amac7->write(AMACreg::LV_ENABLE, 0x0);
//     amac8->write(AMACreg::LV_ENABLE, 0x0);
//     amac9->write(AMACreg::LV_ENABLE, 0x0);    //dc.turnOff();
//     logfile.close();
//  }

//   //get temperature readings
//   std::vector<double> volts;
//   logpath = "log/" + TestName + "_NTC.log";
//   adc_lv1->read({0,1,2,3,4,5,6,7}, volts);
//   for(int ch = 0; ch<8; ch++){
//     if (ch != 6){
//       logfile << volts[ch] << " ";
//     }
//   }

  //
  // Power-off

  ps->turnOff();

  return 0;
}




