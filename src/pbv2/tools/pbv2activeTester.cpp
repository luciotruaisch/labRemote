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

//   logger(logINFO) << "Test LV enable: ";
//   //set DAC values for all 9 
//   dac0->set(0, 0);
//   dac0->set(1, 0);
//   dac0->set(2, 0);
//   dac0->set(3, 0);
//   dac1->set(0, 0);
//   dac1->set(1, 0);
//   dac1->set(2, 0);
//   dac1->set(3, 0);
//   dac2->set(0, 0);
//   dac2->set(1, 0);
//   dac2->set(2, 0);
//   dac2->set(3, 0);
//   //

//   amac2->write(AMACreg::LV_ENABLE, 0x1);
//   std::this_thread::sleep_for(std::chrono::milliseconds(500));
//   double lv_on = tb.getVout(1);//V
//   logger(logINFO) << "yikoes";
//   amac2->write(AMACreg::LV_ENABLE, 0x0);
//   std::this_thread::sleep_for(std::chrono::milliseconds(500));
//   double lv_off = tb.getVout(1);//V

//   bool lv_enable_works=false;
//   if (!(lv_on > 1.4 && lv_on < 1.6 && lv_off < 0.1)) {
//     logger(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
//     lv_enable_works=false;
//   } else {
//     logger(logINFO) << " ++ LV enable good!";
//     lv_enable_works=true;
//   }

//   amac1->write(AMACreg::LV_ENABLE, 0x1);
//   std::this_thread::sleep_for(std::chrono::milliseconds(500));

//   //
//   // Testing Current Sense Amp & Efficiency...

//   double dac_min = 0;
//   double dac_max = (lv_enable_works)?2.75:0;
//   double dac_step = 0.05;

//   logger(logINFO) << "Testing Current Sense Amp & Efficiency...";

//   for (int i = 1; i <= 9; i++)
//   {
//     std::string name;
//     switch (i)
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

//     int DACNum = tb.getDAC_Num(i);
//     int CHVALUE = tb.getDAC_CH(i);
//     logpath = "log/" + name + "_DCDCEfficiency.log";
//     logfile.open(logpath, std::fstream::out);
//     logfile << "Dac Iout IoutADC VoutADC Iin IinADC ntc ptat" << std::endl;

    

//     for (double dac = dac_min; dac <= dac_max; dac+=dac_step) {
//       tb.setDac(DACNum, CHVALUE, dac);
//       unsigned cur = 0;
//       unsigned ptat = 0;
//       unsigned ntc = 0;
//       double dacVal = tb.readDac(DACNum, CHVALUE);
//       double iout = dacVal * 4.0/2.75;
//       std::this_thread::sleep_for(std::chrono::seconds(10));

//       switch(i)
//       {
//         case 1 :
//           amac1->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac1->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac1->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 2 :
//           amac2->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac2->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac2->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 3 :
//           amac3->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac3->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac3->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 4 :
//           amac4->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac4->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac4->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 5 :
//           amac5->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac5->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac5->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 6 :
//           amac6->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac6->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac6->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 7 :
//           amac7->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac7->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac7->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         case 8 :
//           amac8->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac8->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac8->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//           break;

//         default :
//           amac9->read(AMACreg::VALUE_RIGHT_CH1, cur);
//           amac9->read(AMACreg::VALUE_RIGHT_CH3, ptat);
//           amac9->read(AMACreg::VALUE_RIGHT_CH2, ntc);
//           logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps->getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
//     }

//     tb.setDac(DACNum, CHVALUE, 0);
//   }
// }
  
// //set conversion of dac to Iout in this part or in analysis?
//   //I think set it here
//   //write method to get the adc output current 

//   logfile.close();

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
//    //
//   // Testing VIN measurement
//   logger(logINFO) << "Testing VIN measurement ...";

//   logpath = "log/" + TestName + "_VIN.log";
//   logfile.open(logpath, std::fstream::out);
//   logfile << "Vin VinAMAC VinADC" << std::endl;

//   vin_min = 6.0;
//   vin_max = 12.0;
//   vin_step = 0.1;

//   for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
//     ps->setVoltage(vin);
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     unsigned val = 0;
//     amac1->read(AMACreg::VALUE_RIGHT_CH0, val);
//     std::cout << vin << "\t" << val << "\t" << tb.getVin() << std::endl;
//     logfile << vin << " " << val << " " << tb.getVin() << std::endl;
//   }
//   ps->setVoltage(11.0);
//   ps->setCurrent(2.00);
//   logfile.close();

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




