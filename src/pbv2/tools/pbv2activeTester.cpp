#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>

#include <sys/stat.h>
#include <dirent.h>

#include "I2CDevCom.h"
#include "PCA9548ACom.h"
#include "PBv2TB.h"
#include "Logger.h"
#include "ComIOException.h"
#include "DAC5574.h"
#include "AD799X.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "SorensenPs.h"

#ifdef SCOPE
#include "PS6000.h"
#endif

loglevel_e loglevel = logINFO;
//make sure this program is modular - uses pbv2 test class and all of that
//separate different tests into methods, call all of them? 
int main(int argc, char* argv[]) {
  //
  // Get settings from the command line
  if (argc < 11) {
    log(logERROR) << "Not enough parameters!";
    log(logERROR) << "Usage: " << argv[0] << " TESTNAME AMAC1 AMAC2 AMAC3 AMAC4 AMAC5 AMAC6 AMAC7 AMAC8 AMAC9";
    return -1;
  }

  std::string TestName = argv[1];

  
  std::string pb1 = argv[2];
  std::string pb2 = argv[3];
  std::string pb3 = argv[4];
  std::string pb4 = argv[5];
  std::string pb5 = argv[6];
  std::string pb6 = argv[7];
  std::string pb7 = argv[8];
  std::string pb8 = argv[9];
  std::string pb9 = argv[10];


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

  PBv2TB tb;


  log(logINFO) << "Initialising ...";
    
  log(logINFO) << " ... Agilent PS:";
  SorensenPs ps("/dev/ttyUSB0", 11);  
  try
    {
      ps.setCh(2);
      ps.init();
      ps.setVoltage(11);
      ps.setCurrent(5);
      ps.turnOn();
      std::cout << "Power supply turned on, voltage set to " << ps.getVoltage() <<
      " current set to " << ps.getCurrent();
    }
  catch(std::string e)
    {
      log(logERROR) << e;
      return 1;
    }


  //I2C instance mux0
  std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

  std::shared_ptr<I2CCom> amac1_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");

        //three adc
  std::shared_ptr<AD799X> adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
  std::shared_ptr<AD799X> adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
  std::shared_ptr<AD799X> adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));

  std::shared_ptr<DAC5574> dac0=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4C, 1, mux0_com));
  std::shared_ptr<DAC5574> dac1=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4D, 1, mux0_com));
  std::shared_ptr<DAC5574> dac2=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4E, 1, mux0_com));
  
  //first number in AMAC constructor is ID, set to pb number for right now
  //all addresses should not be set to the same, 0x48 does not cause termination
  //
  std::shared_ptr<AMAC> amac1=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,7, mux0_com));
  std::shared_ptr<AMAC> amac2=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,6, mux0_com));
  std::shared_ptr<AMAC> amac3=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,5, mux0_com));
  std::shared_ptr<AMAC> amac4=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,4, mux0_com));
  std::shared_ptr<AMAC> amac5=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,3, mux0_com));
  std::shared_ptr<AMAC> amac6=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,2, mux0_com));
  std::shared_ptr<AMAC> amac7=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,1, mux0_com));
  std::shared_ptr<AMAC> amac8=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,0, mux0_com));
  std::shared_ptr<AMAC> amac9=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x0 ,7, mux0_com));
 
  std::this_thread::sleep_for(std::chrono::milliseconds(200));



  log(logINFO) << "  ++ Baseline values:";
  std::cout << "Input LV Current : " << tb.getVinCurrent() << std::endl;

  unsigned ota_l, ota_r, bgo, dvdd2;
  amac1->write(AMACreg::LV_ENABLE, 0x0);
  amac2->write(AMACreg::LV_ENABLE, 0x0);
  amac3->write(AMACreg::LV_ENABLE, 0x0);
  amac4->write(AMACreg::LV_ENABLE, 0x0);
  amac5->write(AMACreg::LV_ENABLE, 0x0);
  amac6->write(AMACreg::LV_ENABLE, 0x0);
  amac7->write(AMACreg::LV_ENABLE, 0x0);
  amac8->write(AMACreg::LV_ENABLE, 0x0);
  amac9->write(AMACreg::LV_ENABLE, 0x0);

   
  amac1->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac1->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac1->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac1->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb1 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();
    
  amac2->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac2->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac2->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac2->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb2 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();    
    
  amac3->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac3->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac3->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac3->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb3 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  amac4->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac4->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac4->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac4->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb4 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();  

  amac5->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac5->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac5->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac5->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb5 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  amac6->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac6->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac6->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac6->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb6 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();

  amac7->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac7->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac7->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac7->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb7 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();   

  amac8->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac8->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac8->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac8->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb8 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  amac9->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac9->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac9->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac9->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb9 + "_General.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  log(logINFO) << "Test LV enable: ";
  //set DAC values for all 9 
  dac0->set(0, 0);
  dac0->set(1, 0);
  dac0->set(2, 0);
  dac0->set(3, 0);
  dac1->set(0, 0);
  dac1->set(1, 0);
  dac1->set(2, 0);
  dac1->set(3, 0);
  dac2->set(0, 0);
  dac2->set(1, 0);
  dac2->set(2, 0);
  dac2->set(3, 0);
  //

  amac2->write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_on = tb.getVout(1);//V
  log(logINFO) << "yikoes";
  amac2->write(AMACreg::LV_ENABLE, 0x0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_off = tb.getVout(1);//V

  bool lv_enable_works=false;
  if (!(lv_on > 1.4 && lv_on < 1.6 && lv_off < 0.1)) {
    log(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
    lv_enable_works=false;
  } else {
    log(logINFO) << " ++ LV enable good!";
    lv_enable_works=true;
  }

  amac1->write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  //
  // Testing Current Sense Amp & Efficiency...

  double dac_min = 0;
  double dac_max = (lv_enable_works)?2.75:0;
  double dac_step = 0.05;

  log(logINFO) << "Testing Current Sense Amp & Efficiency...";

  for (int i = 1; i <= 9; i++)
  {
    std::string name;
    switch (i)
    {
    case 1 : 
      name = TestName + "_" + pb1;
      break;
    case 2 : 
      name = TestName + "_" + pb2;
      break;
    case 3 : 
      name = TestName + "_" + pb3;
      break;
    case 4 : 
      name = TestName + "_" + pb4;
      break;
    case 5 : 
      name = TestName + "_" + pb5;
      break;
    case 6 : 
      name = TestName + "_" + pb6;
      break;
    case 7 : 
      name = TestName + "_" + pb7;
      break;
    case 8 : 
      name = TestName + "_" + pb8;
      break;
    default: 
      name = TestName + "_" + pb9;
    }

    int DACNum = tb.getDAC_Num(i);
    int CHVALUE = tb.getDAC_CH(i);
    logpath = "log/" + name + "_DCDCEfficiency.log";
    logfile.open(logpath, std::fstream::out);
    logfile << "Dac Iout IoutADC VoutADC Iin IinADC ntc ptat" << std::endl;

    

    for (double dac = dac_min; dac <= dac_max; dac+=dac_step) {
      tb.setDac(DACNum, CHVALUE, dac);
      unsigned cur = 0;
      unsigned ptat = 0;
      unsigned ntc = 0;
      double dacVal = tb.readDac(DACNum, CHVALUE);
      double iout = dacVal * 4.0/2.75;
      std::this_thread::sleep_for(std::chrono::seconds(10));

      switch(i)
      {
        case 1 :
          amac1->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac1->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac1->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 2 :
          amac2->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac2->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac2->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 3 :
          amac3->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac3->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac3->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 4 :
          amac4->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac4->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac4->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 5 :
          amac5->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac5->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac5->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 6 :
          amac6->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac6->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac6->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 7 :
          amac7->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac7->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac7->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 8 :
          amac8->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac8->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac8->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        default :
          amac9->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac9->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac9->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << dacVal << " " << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
    }

    tb.setDac(DACNum, CHVALUE, 0);
  }
}
  
//set conversion of dac to Iout in this part or in analysis?
  //I think set it here
  //write method to get the adc output current 

  logfile.close();

  //
  // Testing Current Sense Amp & Efficiency...
  double vin_min = 4.0;
  double vin_max = 5.0;
  double vin_step = 0.05;


  for(int j = 1; j<=9; j++){
    std::string name;
    switch (j)
    {
    case 1 : 
      name = TestName + "_" + pb1;
      break;
    case 2 : 
      name = TestName + "_" + pb2;
      break;
    case 3 : 
      name = TestName + "_" + pb3;
      break;
    case 4 : 
      name = TestName + "_" + pb4;
      break;
    case 5 : 
      name = TestName + "_" + pb5;
      break;
    case 6 : 
      name = TestName + "_" + pb6;
      break;
    case 7 : 
      name = TestName + "_" + pb7;
      break;
    case 8 : 
      name = TestName + "_" + pb8;
      break;
    default: 
      name = TestName + "_" + pb9;
    }

    log(logINFO) << "Testing current usage as a funciton of input voltage...";
    //int DACNum = tb.getDAC_Num(j);
    //int CHVALUE = tb.getDAC_CH(j);
    logpath = "log/" + name + "_VinIin.log";
    logfile.open(logpath, std::fstream::out);
    logfile << "Vin Iin IinADC Vout" << std::endl;

    for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
      ps.turnOff();
      ps.setVoltage(vin);
      ps.turnOn();
      amac1->write(AMACreg::LV_ENABLE, 0x1);
      amac2->write(AMACreg::LV_ENABLE, 0x1);
      amac3->write(AMACreg::LV_ENABLE, 0x1);
      amac4->write(AMACreg::LV_ENABLE, 0x1);
      amac5->write(AMACreg::LV_ENABLE, 0x1);
      amac6->write(AMACreg::LV_ENABLE, 0x1);
      amac7->write(AMACreg::LV_ENABLE, 0x1);
      amac8->write(AMACreg::LV_ENABLE, 0x1);
      amac9->write(AMACreg::LV_ENABLE, 0x1);

      std::this_thread::sleep_for(std::chrono::seconds(2));

      //double lv = dc.getValues().vol;//mV
      //should this be DAC values or tb.getVinCurrent()?
      double lvADC = tb.getVout(j); //get vout
      std::cout << vin << "\t" << ps.getCurrent() << "\t" << tb.getVinCurrent() << "\t" << lvADC << std::endl;
      logfile << vin << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << lvADC << std::endl;
    }
    amac1->write(AMACreg::LV_ENABLE, 0x0);
    amac2->write(AMACreg::LV_ENABLE, 0x0);
    amac3->write(AMACreg::LV_ENABLE, 0x0);
    amac4->write(AMACreg::LV_ENABLE, 0x0);
    amac5->write(AMACreg::LV_ENABLE, 0x0);
    amac6->write(AMACreg::LV_ENABLE, 0x0);
    amac7->write(AMACreg::LV_ENABLE, 0x0);
    amac8->write(AMACreg::LV_ENABLE, 0x0);
    amac9->write(AMACreg::LV_ENABLE, 0x0);    //dc.turnOff();
    logfile.close();
 }
   //
  // Testing VIN measurement
  log(logINFO) << "Testing VIN measurement ...";

  logpath = "log/" + TestName + "_VIN.log";
  logfile.open(logpath, std::fstream::out);
  logfile << "Vin VinAMAC VinADC" << std::endl;

  vin_min = 6.0;
  vin_max = 12.0;
  vin_step = 0.1;

  for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
    ps.setVoltage(vin);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    unsigned val = 0;
    amac1->read(AMACreg::VALUE_RIGHT_CH0, val);
    std::cout << vin << "\t" << val << "\t" << tb.getVin() << std::endl;
    logfile << vin << " " << val << " " << tb.getVin() << std::endl;
  }
  ps.setVoltage(11.0);
  ps.setCurrent(2.00);
  logfile.close();

  //get temperature readings
  std::vector<double> volts;
  logpath = "log/" + TestName + "_NTC.log";
  adc_lv1->read({0,1,2,3,4,5,6,7}, volts);
  for(int ch = 0; ch<8; ch++){
    if (ch != 6){
      logfile << volts[ch] << " ";
    }
  }

  //
  // Power-off

  ps.turnOff();

    return 0;
}




