#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "I2CFTDICom.h"
#endif

#include "Logger.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "Bk85xx.h"
#include "AgilentPs.h"
#include "Keithley24XX.h"

#ifdef SCOPE
#include "PS6000.h"
#endif

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[])
{
#ifndef FTDI
  logger(logERROR) << "FTDI support not enabled.";
  return -1;
#else  
  //
  // Get settings from the command line
  if (argc < 4) {
    logger(logERROR) << "Not enough parameters!";
    logger(logERROR) << "Usage: " << argv[0] << " TESTNAME <BK85XX> <GPIB>";
    return -1;
  }

  std::string TestName = argv[1];
  std::string bkDev = argv[2];
  std::string gpibDev = argv[3];

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
  std::string logpath;
  std::fstream logfile;

  logger(logINFO) << "Initialising ...";
    
  logger(logINFO) << " ... Agilent PS:";
  AgilentPs ps(gpibDev, 10);
  try
    {
      ps.init();
      ps.setRange(20);
      ps.setVoltage(11.0);
      ps.setCurrent(2.00);
      ps.turnOn();
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  logger(logINFO) << " ... Keithley 2410:";
  Keithley24XX sm(gpibDev, 9);
  try
    {
      sm.init();
      sm.setSource(KeithleyMode::CURRENT, 1e-6, 1e-6);
      sm.setSense(KeithleyMode::VOLTAGE, 500, 500);
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  logger(logINFO) << " ... DC Load:";
  Bk85xx dc(bkDev);
  dc.setRemote();
  dc.setRemoteSense(false);
  dc.setModeCC();
  dc.setCurrent(0);
  dc.turnOn();

  // Configure pico scope
  logger(logINFO) << " ... PicoScope Load:";
#ifdef SCOPE
  PS6000 pico;
  pico.open();
  pico.setEnable(1,false);
  pico.setEnable(2,false);
  pico.setEnable(3,false);
  pico.setPeriod(1e-8);
  pico.setRange(0, 200);
  pico.configChannels();
  float period=pico.getPeriod();
  std::vector<std::vector<float>> picodata(1,std::vector<float>(PS6000_BUFFER_SIZE));
#else // SCOPE
  logger(logINFO) << "\tSkipping noise measurement due to missing libScope";
#endif // SCOPE

  logger(logINFO) << " ... AMAC:";
  std::shared_ptr<I2CCom> i2c=std::make_shared<I2CFTDICom>(0x0);
  AMAC amac(i2c);

  logger(logINFO) << "  ++Init";
  amac.init();

  logger(logINFO) << "  ++Disable LV";
  amac.write(AMACreg::LV_ENABLE, 0x0);
  logger(logINFO) << "  ++Disable HV";
  amac.write(AMACreg::HV_ENABLE, 0x0);


  logger(logINFO) << "  ++ Const ADC values:";
  unsigned ota_l, ota_r, bgo, dvdd2;
  amac.read(AMACreg::VALUE_LEFT_CH5, ota_l);
  std::cout << "OTA LEFT : \t" << ota_l << std::endl;
  amac.read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  std::cout << "OTA_RIGHT : \t" << ota_r << std::endl;
  amac.read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  std::cout << "DVDD/2 : \t" << dvdd2 << std::endl;
  amac.read(AMACreg::VALUE_LEFT_CH2, bgo);
  std::cout << "BGO : \t" << bgo << std::endl;

  logger(logINFO) << "  ++ Baseline values:";
  std::cout << "Input LV Current : " << ps.getCurrent() << std::endl;

  logpath = "log/" + TestName + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << ps.getCurrent() << std::endl;
  logfile.close();

  logger(logINFO) << "Test LV enable: ";
  dc.setCurrent(0);
  dc.turnOn();

  amac.write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_on = dc.getValues().vol;//mV

  amac.write(AMACreg::LV_ENABLE, 0x0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_off = dc.getValues().vol;//mV

  bool lv_enable_works=false;
  if (!(lv_on > 1.4e3 && lv_on < 1.6e3 && lv_off < 0.1e3)) {
    logger(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
    lv_enable_works=false;
  } else {
    logger(logINFO) << " ++ LV enable good!";
    lv_enable_works=true;
  }

  amac.write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  //
  // Testing Current Sense Amp & Efficiency...
  logger(logINFO) << "Testing Current Sense Amp & Efficiency...";

  logpath = "log/" + TestName + "_DCDCEfficiency.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "Iout Vout Iin Vin IoutADC ntc ptat" << std::endl;

  double iout_min = 0;
  double iout_max = (lv_enable_works)?4000:0;
  double iout_step = 100;

  for (double iout = iout_min; iout <= iout_max; iout+=iout_step) {
    dc.setCurrent(iout);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    unsigned cur = 0;
    amac.read(AMACreg::VALUE_RIGHT_CH1, cur);
    unsigned ptat = 0;
    amac.read(AMACreg::VALUE_RIGHT_CH3, ptat);
    unsigned ntc = 0;
    amac.read(AMACreg::VALUE_RIGHT_CH2, ntc);
    std::cout << iout << "\t" << dc.getValues().vol << "\t" << cur << "\t" << ps.getCurrent() << "\t" << ntc << "\t" << ptat << std::endl;
    logfile << iout << " " << dc.getValues().vol << " " << ps.getCurrent() << " " << ps.getVoltage() << " " << cur << " " << ntc << " " << ptat << std::endl;
  }
  dc.setCurrent(1000);
  dc.turnOff();
  logfile.close();

  //
  // Testing Current Sense Amp & Efficiency...
  logger(logINFO) << "Testing current usage as a funciton of input voltage...";

  logpath = "log/" + TestName + "_VinIin.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "Vin\tIin\tVout" << std::endl;

  double vin_min = 4.0;
  double vin_max = 5.0;
  double vin_step = 0.05;

  dc.setCurrent(100);
  dc.turnOn();

  for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
    ps.turnOff();
    ps.setVoltage(vin);
    ps.turnOn();
    amac.init();
    amac.write(AMACreg::LV_ENABLE, 0x1);
    std::this_thread::sleep_for(std::chrono::seconds(2));

    double lv = dc.getValues().vol;//mV
    std::cout << vin << "\t" << ps.getCurrent() << "\t" << lv << std::endl;
    logfile << vin << "\t" << ps.getCurrent() << "\t" << lv << std::endl;
  }
  amac.write(AMACreg::LV_ENABLE, 0x0);
  dc.turnOff();
  logfile.close();

  //
  // Testing VIN measurement
  logger(logINFO) << "Testing VIN measurement ...";

  logpath = "log/" + TestName + "_VIN.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "Vin VinADC" << std::endl;

  vin_min = 6.0;
  vin_max = 12.0;
  vin_step = 0.1;

  for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
    ps.setVoltage(vin);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    unsigned val = 0;
    amac.read(AMACreg::VALUE_RIGHT_CH0, val);
    std::cout << vin << "\t" << val << std::endl;
    logfile << vin << " " << val << std::endl;
  }
  ps.setVoltage(11.0);
  ps.setCurrent(2.00);
  logfile.close();

  //
  // Testing Ileak measurement
  logger(logINFO) << "Testing Ileak measurement ...";

  logpath = "log/" + TestName + "_Ileak.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "HV Ileak OpAmpGain0 OpAmpGain1 OpAmpGain2 OpAmpGain4 OpAmpGain8" << std::endl;

  amac.write(AMACreg::HV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(3000));
  double ileak_min = 1e-6;
  double ileak_max = 1e-3;
  double ileak_step = 1e-6;
  sm.setSource(KeithleyMode::CURRENT, ileak_min, ileak_min);
  sm.turnOn();

  int counter = 0;
  for (double ileak=ileak_min; ileak<=ileak_max; ileak += ileak_step) {
    counter++;
    if (counter%10 == 0)
      ileak_step = ileak_step*10;
    sm.setSource(KeithleyMode::CURRENT, ileak, ileak);
    std::string hv=sm.sense(KeithleyMode::VOLTAGE);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    unsigned val[5];
    for (unsigned i=0; i<5; i++) {
      if (i == 0)
  	amac.write(AMACreg::OPAMP_GAIN_LEFT, 0);
      else
  	amac.write(AMACreg::OPAMP_GAIN_LEFT, pow(2,i-1));

      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      amac.read(AMACreg::VALUE_LEFT_CH6, *&val[i]);
    }
    std::cout << hv << "\t" << ileak << "\t" << val[0] << "\t" << val[1] << "\t" << val[2] << "\t" << val[3] << "\t" << val[4] << std::endl;
    logfile << hv << " " << ileak << " " << val[0] << " " << val[1] << " " << val[2] << " " << val[3] << " " << val[4] << std::endl;
  }
  logfile.close();

  //
  // Testing shieldbox leakage
#ifdef SCOPE
  logger(logINFO) << "Testing shield box leakage ...";

  // Run test with LV on
  logpath = "log/" + TestName + "_CoilLVON.log";
  logfile.open(logpath, std::fstream::out);

  amac.write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  pico.run(picodata);

  logfile << "time coil" << std::endl;
  for(unsigned int row=0; row<picodata[0].size(); row++)
    logfile << row*period << " " << picodata[0][row] << std::endl;

  logfile.close();

  // Run test with LV off
  logpath = "log/" + TestName + "_CoilLVOFF.log";
  logfile.open(logpath, std::fstream::out);

  amac.write(AMACreg::LV_ENABLE, 0x0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  pico.run(picodata);

  logfile << "time coil" << std::endl;
  for(unsigned int row=0; row<picodata[0].size(); row++)
    logfile << row*period << " " << picodata[0][row] << std::endl;

  logfile.close();
#endif // SCOPE

  //
  // Power-off
#ifdef SCOPE
  pico.close();
#endif // SCOPE
  sm.turnOff();
  ps.turnOff();
  dc.turnOff();
    
  /*
    logger(logINFO) << "  ++ADC Values";
    unsigned val = 0;
    amac.read(AMACreg::VALUE_RIGHT_CH0, val);
    logger(logINFO) << "[VIN] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH1, val);
    logger(logINFO) << "[IOUT] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH2, val);
    logger(logINFO) << "[NTC_Temp] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH3, val);
    logger(logINFO) << "[3R] : \t\t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH4, val);
    logger(logINFO) << "[VDD_H/4] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH5, val);
    logger(logINFO) << "[OTA] : \t" << val ;
    amac.read(AMACreg::VALUE_RIGHT_CH6, val);
    logger(logINFO) << "[ICHANR] : \t" << val ;
    
    amac.read(AMACreg::VALUE_LEFT_CH0, val);
    logger(logINFO) << "[0L] : \t\t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH1, val);
    logger(logINFO) << "[DVDD/2] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH2, val);
    logger(logINFO) << "[BGO] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH3, val);
    logger(logINFO) << "[3L] : \t\t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH4, val);
    logger(logINFO) << "[TEMP] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH5, val);
    logger(logINFO) << "[OTA] : \t" << val ;
    amac.read(AMACreg::VALUE_LEFT_CH6, val);
    logger(logINFO) << "[ICHANL] : \t" << val ;

    */
#endif // FTDI
  return 0;
}
