#include <memory>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#include <sys/stat.h>
#include <dirent.h>

#include "I2CDevCom.h"
#include "PCA9548ACom.h"
#include "PBv2TB.h"
#include "Logger.h"
#include "ComIOException.h"
#include "DAC5574.h"
#include "AD799X.h"
#include "MCP3428.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "AMAC_icalibrate.h"
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
  if (argc < 20) {
    logger(logERROR) << "Not enough parameters!";
    logger(logERROR) << "Usage: " << argv[0] << " TESTNAME AMAC1 K01 AMAC2 K02 AMAC3 L01 AMAC4 L02 AMAC5 L03 AMAC6 L04 AMAC7 K03 AMAC8 K04 AMAC9 K05";
    return -1;
  }

  //Test name
  std::string TestName = argv[1];

  //Number of PB to be tested
  unsigned NumPB = 7;

  std::vector<std::string> pbName(NumPB);
  std::vector<std::string> AMACName(NumPB);

  //Specify AMAC name
  for(unsigned i=0;i<NumPB;i++)
    {
      pbName[i] = argv[2*i+2];
      std::cout << pbName[i] << " ";
      AMACName[i] = argv[2*i+3];
      std::cout << AMACName[i] << std::endl;
    }
  /*
  std::string pb1 = argv[2];
  std::string pb2 = argv[3];
  std::string pb3 = argv[4];
  std::string pb4 = argv[5];
  std::string pb5 = argv[6];
  std::string pb6 = argv[7];
  std::string pb7 = argv[8];
  std::string pb8 = argv[9];
  std::string pb9 = argv[10];*/


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


  logger(logINFO) << "Initialising ...";
    
  logger(logINFO) << " ... Agilent PS:";
  SorensenPs ps("/dev/ttyUSB0", 11);  
  try
    {
      ps.setCh(1);
      ps.init();
      ps.setVoltage(11);
      ps.setCurrent(5);
      ps.turnOn();
      std::cout << "Power supply turned on, voltage set to " << ps.getVoltage() <<
	" current set to " << ps.getCurrent() << std::endl;
    }
  catch(std::string e)
    {
      logger(logERROR) << e;
      return 1;
    }

  // Configure pico scope

/*
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
  std::vector<std::vector<float>> picodata;
#else // SCOPE
  logger(logINFO) << "\t Skipping noise measurement due to missing libScope";
#endif // SCOPE
*/ 

  //I2C instance mux0
  std::shared_ptr<I2CCom> mux0_com=std::make_shared<I2CDevCom>(0x70, "/dev/i2c-0");
  std::shared_ptr<I2CCom> mux1_com=std::make_shared<I2CDevCom>(0x74, "/dev/i2c-0");

        //three adc
  std::shared_ptr<AD799X> adc_pwr=std::make_shared<AD799X>(3.3, AD799X::AD7993, std::make_shared<PCA9548ACom>(0x22, 2, mux0_com));
  std::shared_ptr<AD799X> adc_lv0=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x21, 1, mux0_com));
  std::shared_ptr<AD799X> adc_lv1=std::make_shared<AD799X>(3.3, AD799X::AD7997, std::make_shared<PCA9548ACom>(0x22, 1, mux0_com));
  std::shared_ptr<MCP3428> m_adc_hv0=std::make_shared<MCP3428>(2.048, MCP3428::e12_bit, MCP3428::eShot, MCP3428::e_x1, std::make_shared<PCA9548ACom>(0x68, 2, mux0_com));
  std::shared_ptr<MCP3428> m_adc_hv1=std::make_shared<MCP3428>(2.048, MCP3428::e12_bit, MCP3428::eShot, MCP3428::e_x1, std::make_shared<PCA9548ACom>(0x6C, 2 ,mux0_com));
  std::shared_ptr<MCP3428> m_adc_hv2=std::make_shared<MCP3428>(2.048, MCP3428::e12_bit, MCP3428::eShot, MCP3428::e_x1, std::make_shared<PCA9548ACom>(0x6A, 2, mux1_com));

  std::shared_ptr<DAC5574> dac0=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4C, 1, mux0_com));
  std::shared_ptr<DAC5574> dac1=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4D, 1, mux0_com));
  std::shared_ptr<DAC5574> dac2=std::make_shared<DAC5574>(3.3, std::make_shared<PCA9548ACom>(0x4E, 1, mux0_com));
  
  //Initialize AMAC
  std::vector<std::shared_ptr<AMAC>> amac(NumPB);

  amac[0]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,7, mux1_com));
  amac[1]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,6, mux1_com));
  amac[2]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,5, mux1_com));
  amac[3]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,4, mux1_com));
  amac[4]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,3, mux1_com));
  amac[5]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,2, mux1_com));
  amac[6]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,1, mux1_com));
  //amac[7]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,0, mux1_com));
  //amac[8]=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,7, mux0_com));
 
  /* std::cout << "set AMAC addresses 04" << std::endl;
  std::shared_ptr<AMAC> amac1=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,7, mux1_com));
  std::cout << "set first AMAC address" << std::endl;
  std::shared_ptr<AMAC> amac2=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,6, mux1_com));
  std::cout << "set second AMAC address" << std::endl;  
  std::shared_ptr<AMAC> amac3=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,5, mux1_com));
  std::cout << "set third AMAC address" << std::endl;  
  std::shared_ptr<AMAC> amac4=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,4, mux1_com));
  std::cout << "set fourth AMAC address" << std::endl;  
  std::shared_ptr<AMAC> amac5=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,3, mux1_com));
  std::cout << "set fifth AMAC address" << std::endl;  
  std::shared_ptr<AMAC> amac6=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,2, mux1_com));
  std::cout << "set sixth AMAC address" << std::endl;  
  std::shared_ptr<AMAC> amac7=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,1, mux1_com));
  std::cout << "set seventh AMAC address" << std::endl;  
  std::shared_ptr<AMAC> amac8=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,0, mux1_com));
  std::cout << "set eigth AMAC address" << std::endl;
  std::shared_ptr<AMAC> amac9=std::make_shared<AMAC>(std::make_shared<PCA9548ACom>(0x00 ,7, mux0_com));
  std::cout << "set ninth AMAC address" << std::endl;
  */
  //Initialization of AMAC HV leakage measurement
  std::vector<std::shared_ptr<AMAC_icalibrate>> amac_icalib(9);

  amac_icalib[0]=std::make_shared<AMAC_icalibrate>(AMACName[0]);
  amac_icalib[1]=std::make_shared<AMAC_icalibrate>(AMACName[1]);
  amac_icalib[2]=std::make_shared<AMAC_icalibrate>(AMACName[2]);
  amac_icalib[3]=std::make_shared<AMAC_icalibrate>(AMACName[3]);
  amac_icalib[4]=std::make_shared<AMAC_icalibrate>(AMACName[4]);
  amac_icalib[5]=std::make_shared<AMAC_icalibrate>(AMACName[5]);
  amac_icalib[6]=std::make_shared<AMAC_icalibrate>(AMACName[6]);
  //amac_icalib[7]=std::make_shared<AMAC_icalibrate>(AMACName[7]);
  //amac_icalib[8]=std::make_shared<AMAC_icalibrate>(AMACName[8]);

  /*std::shared_ptr<AMAC_icalibrate> amac1_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0);
  std::shared_ptr<AMAC_icalibrate> amac2_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac3_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac4_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac5_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac6_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac7_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac8_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); 
  std::shared_ptr<AMAC_icalibrate> amac9_icalib=std::make_shared<AMAC_icalibrate>("L01",AMAC_icalibrate::LEFT,0,0,0); */

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  //amacs for all?

  //Do the test 1000times---!!!modified
  for(int t = 0; t < 1; t++)
  {
  clock_t begin = clock();
  logger(logINFO) << "  ++ Baseline values:";
  std::cout << "Input LV Current : " << tb.getVinCurrent() << std::endl;
  
  unsigned ota_l, ota_r, bgo, dvdd2;
  //Disable of all AMAC LV
  for(unsigned i=2;i<3;i++)
    {
      amac[i]->write(AMACreg::LV_ENABLE, 0x0);
    }
  /* amac1->write(AMACreg::LV_ENABLE, 0x0);
  amac2->write(AMACreg::LV_ENABLE, 0x0);
  amac3->write(AMACreg::LV_ENABLE, 0x0);
  amac4->write(AMACreg::LV_ENABLE, 0x0);
  amac5->write(AMACreg::LV_ENABLE, 0x0);
  amac6->write(AMACreg::LV_ENABLE, 0x0);
  amac7->write(AMACreg::LV_ENABLE, 0x0);
  // amac8->write(AMACreg::LV_ENABLE, 0x0);
  //amac9->write(AMACreg::LV_ENABLE, 0x0);*/

  //Measure standby mode
  for(unsigned i=2;i<3;i++)
    {
      //Read OTA value left
      amac[i]->read(AMACreg::VALUE_LEFT_CH5, ota_l);

      //Read OTA value right
      amac[i]->read(AMACreg::VALUE_RIGHT_CH5, ota_r);

      //Read DVDD2
      amac[i]->read(AMACreg::VALUE_LEFT_CH1, dvdd2);

      //Read BGO
      amac[i]->read(AMACreg::VALUE_LEFT_CH2, bgo);

      logpath = "log/" + TestName + "_" + pbName[i] + "_General" + std::to_string(t) + ".log";
      logfile.open(logpath, std::fstream::out);
      logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
      logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
      logfile.close();
    }

  /*amac1->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac1->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac1->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac1->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb1 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();
    
  amac2->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac2->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac2->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac2->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb2 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();    
    
  amac3->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac3->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac3->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac3->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb3 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  amac4->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac4->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac4->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac4->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb4 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();  

  amac5->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac5->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac5->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac5->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb5 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  amac6->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac6->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac6->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac6->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb6 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();

  amac7->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac7->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac7->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac7->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb7 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();   

  /* amac8->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac8->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac8->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac8->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb8 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close(); 

  amac9->read(AMACreg::VALUE_LEFT_CH5, ota_l);
  amac9->read(AMACreg::VALUE_RIGHT_CH5, ota_r);
  amac9->read(AMACreg::VALUE_LEFT_CH1, dvdd2);
  amac9->read(AMACreg::VALUE_LEFT_CH2, bgo);
  logpath = "log/" + TestName + "_" + pb9 + "_General" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "OTALEFT OTARIGHT DVDD2 BGO InBase" << std::endl;
  logfile << ota_l << " " << ota_r << " " << dvdd2 << " " << bgo << " " << tb.getVinCurrent() << std::endl;
  logfile.close();    */


  logger(logINFO) << "Test LV enable: ";
  //set all variable LOAD to max 
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

  //Test LV enable
  std::vector<bool> lv_enable_works(NumPB);

  for(unsigned i=2;i<3;i++)
    {
      amac[i]->write(AMACreg::LV_ENABLE, 0x1);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      double lv_on = tb.getVout(i);//V
      
      amac[i]->write(AMACreg::LV_ENABLE, 0x0);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      double lv_off = tb.getVout(i);//V
      
      lv_enable_works[i]=false;
      if (!(lv_on > 1.4 && lv_on < 1.6 && lv_off < 0.1)) {
	logger(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
	lv_enable_works[i]=false;
      } else {
	 logger(logINFO) << " ++ LV enable good!";
	 lv_enable_works[i]=true;
      }
    }


  /*amac1->write(AMACreg::LV_ENABLE, 0x1);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_on = tb.getVin();//V

  amac1->write(AMACreg::LV_ENABLE, 0x0);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  double lv_off = tb.getVin();//V

  bool lv_enable_works=false;
  if (!(lv_on > 1.4 && lv_on < 1.6 && lv_off < 0.1)) {
    logger(logERROR) << " ++ LV enable not working! " << lv_on << " " << lv_off;
    lv_enable_works=false;
  } else {
    logger(logINFO) << " ++ LV enable good!";
    lv_enable_works=true;
    }*/

  for(unsigned i=2;i<3;i++)
    {
      amac[i]->write(AMACreg::LV_ENABLE, 0x1);
    }

  /* amac1->write(AMACreg::LV_ENABLE, 0x1);
  amac2->write(AMACreg::LV_ENABLE, 0x1);
  amac3->write(AMACreg::LV_ENABLE, 0x1);
  amac4->write(AMACreg::LV_ENABLE, 0x1);
  amac5->write(AMACreg::LV_ENABLE, 0x1);
  amac6->write(AMACreg::LV_ENABLE, 0x1);
  amac7->write(AMACreg::LV_ENABLE, 0x1);
  // amac8->write(AMACreg::LV_ENABLE, 0x1);
  //amac9->write(AMACreg::LV_ENABLE, 0x1);*/


  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  //
  // Testing Current Sense Amp & Efficiency...
  double dac_min = 0;
  std::vector<double> dac_max(NumPB);
  for(unsigned i=2;i<3;i++)
    {
      dac_max[i] = (lv_enable_works[i])?2.75:0;
    }
  double dac_step = 0.05;

  logger(logINFO) << "Testing Current Sense Amp & Efficiency...";

  for (unsigned i = 2; i < 3; i++)
  {
    std::string name = TestName + "_" + pbName[i];
   /*switch (i)
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
      }*/

    logpath = "log/" + name + "_DCDCEfficiencyBurnin" + std::to_string(t) + ".log";
    logfile.open(logpath, std::fstream::out);
    logfile << "Dac Iout IoutADC VoutADC Iin IinADC ntc ptat" << std::endl;

    
  
    for (double dac = dac_min; dac <= dac_max[i]; dac+=dac_step) 
      {
	unsigned cur = 0;
	unsigned ptat = 0;
	unsigned ntc = 0;
	double iout = tb.setLoad(i, dac);
	std::this_thread::sleep_for(std::chrono::seconds(1));

	amac[i]->read(AMACreg::VALUE_RIGHT_CH1, cur);
	amac[i]->read(AMACreg::VALUE_RIGHT_CH3, ptat);
	amac[i]->read(AMACreg::VALUE_RIGHT_CH2, ntc);
	logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;

      /*switch(i)
      {
        case 1 :
          amac1->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac1->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac1->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 2 :
          amac2->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac2->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac2->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 3 :
          amac3->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac3->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac3->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 4 :
          amac4->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac4->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac4->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 5 :
          amac5->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac5->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac5->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 6 :
          amac6->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac6->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac6->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        case 7 :
          amac7->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac7->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac7->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

	  /* case 8 :
          amac8->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac8->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac8->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
          break;

        default :
          amac9->read(AMACreg::VALUE_RIGHT_CH1, cur);
          amac9->read(AMACreg::VALUE_RIGHT_CH3, ptat);
          amac9->read(AMACreg::VALUE_RIGHT_CH2, ntc);
          logfile << iout << " " << cur << " " << tb.getVout(i) << " " << ps.getCurrent() << " " << tb.getVinCurrent() << " " << ntc << " " << ptat << std::endl;
      default:
	break;
    }*/
      }
    logfile.close();
  }


  //logfile.close();

  //
  // Testing Current Sense Amp & Efficiency...
  double vin_min = 4.0;
  double vin_max = 5.0;
  double vin_step = 0.05;


  for(unsigned i= 2; i<3; i++){
    std::string name = TestName + "_" + pbName[i];
    /*switch (j)
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
      }*/

    logger(logINFO) << "Testing current usage as a funciton of input voltage...";
    logpath = "log/" + name + "_VinIin" + std::to_string(t) + ".log";
    logfile.open(logpath, std::fstream::out);
    logfile << "Vin Iin IinADC" << std::endl;

    for (double vin=vin_min; vin<=vin_max; vin+=vin_step) {
      ps.turnOff();
      ps.setVoltage(vin);
      ps.turnOn();
      amac[i]->init();
      amac[i]->write(AMACreg::LV_ENABLE, 0x1);
      std::this_thread::sleep_for(std::chrono::seconds(2));

      std::cout << vin << "\t" << ps.getCurrent() << "\t" << tb.getVinCurrent() << std::endl;
      logfile << vin << " " << ps.getCurrent() << " " << tb.getVinCurrent() << std::endl;

      amac[i]->write(AMACreg::LV_ENABLE, 0x0);
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    //dc.turnOff();
    logfile.close();
 }
   //
  // Testing VIN measurement

  logger(logINFO) << "Testing VIN measurement ...";

  logpath = "log/" + TestName + "_VIN" + std::to_string(t) + ".log";
  logfile.open(logpath, std::fstream::out);
  logfile << "Vin VinAMAC VinADC" << std::endl;

  vin_min = 6.0;
  vin_max = 12.0;
  vin_step = 0.1;

  for(unsigned i=2;i<3;i++)
    {

      for (double vin=vin_min; vin<=vin_max; vin+=vin_step) 
	{
	  ps.setVoltage(vin);
	  std::this_thread::sleep_for(std::chrono::seconds(1));
	  unsigned val = 0;
	  amac[i]->read(AMACreg::VALUE_RIGHT_CH0, val);
	  std::cout << vin << "\t" << val << "\t" << tb.getVin() << std::endl;
	  logfile << vin << " " << val << " " << tb.getVin() << std::endl;
	}
    }
  ps.setVoltage(11.0);
  ps.setCurrent(2.00);
  logfile.close();

  //get temperature readings
  std::vector<double> volts;
  logpath = "log/" + TestName + "_NTC" + std::to_string(t) + ".log";
  adc_lv1->read({0,1,2,3,4,5,6,7}, volts);
  for(int ch = 0; ch<8; ch++){
    if (ch != 6){
      logfile << volts[ch] << " ";
    }
  }

  //Testing the high voltage
  for(unsigned i = 2; i<3; i++)
{
    std::string name = TestName + "_" + pbName[i];
    /*switch (j)
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
      }*/

    logger(logINFO) << "Testing the high voltage";
    logpath = "log/" + name + "_HV" + std::to_string(t) + ".log";
    logfile.open(logpath, std::fstream::out);
    logfile << "HVin HVadc AMACleakage ADCleakage" << std::endl;
  
    //Specified the HV applied voltage
    double HVmin = 0.0;
    double HVmax = 30.0;

    ps.setCh(2);
    for(double HV=HVmin;HV<=HVmax;HV+=HVmin)
      {
	ps.turnOff();
	ps.setVoltage(HV);
	ps.turnOn();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	
	double HV_ADC;
	switch(i)
	  {
	  case 0:
	    HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB1);
	    break;
	  case 1:
	    HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB2);
	    break;
	  case 2:
	    HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB3);
	    break;
	  case 3:
	    HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB4);
	    break;
	  case 4:
	    HV_ADC = m_adc_hv1->read(ADC_HVOUT_PB5);
	    break;
	  case 5:
	    HV_ADC = m_adc_hv1->read(ADC_HVOUT_PB6);
	    break;
	  case 6:
	    HV_ADC = m_adc_hv1->read(ADC_HVOUT_PB7);
	    break;
	  case 7:
	    HV_ADC = m_adc_hv1->read(ADC_HVOUT_PB8);
	    break;
	  case 8:
	    HV_ADC = m_adc_hv1->read(ADC_HVOUT_PB9);
	    break;
	  default:
	    HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB1);
	    break;
	  }

        double HV_supply =  HV_ADC*550/30;
	double Ileake_ADC = HV_ADC/(30.0*pow(10.0,3.0));
	uint8_t OpAmpGain_init;
	uint8_t OpAmpGainMax;

	//Read BandGapControl
	unsigned BandGap;
	amac[i]->read(AMACreg::BANDGAP_CONTROL,BandGap);

	//Read RampGain
	unsigned RampGain;
	amac[i]->read(AMACreg::LEFT_RAMP_GAIN,RampGain);
	if(Ileake_ADC < 20e-6)
	  {
	    OpAmpGain_init = 0;
	    OpAmpGainMax = 2;
	  }      
	else if((Ileake_ADC >= 20e-6)&&(Ileake_ADC < 1e-3))
	  {
	    OpAmpGain_init = 2;
	    OpAmpGainMax = 4;
	  }
	else if ((Ileake_ADC >= 1e-3)&&(Ileake_ADC < 7e-3))
	  {
	    OpAmpGain_init = 4;
	    OpAmpGainMax = 16;
	  }
	else
	  {
	    OpAmpGain_init = 0;
	    OpAmpGainMax = 16;
	    std::cout << "Measured Leakage current is not in the range" << std::endl;
	  }
	//Do a mean value of the measured leakage current
	double I_gain = 0.0;
	for(unsigned j=OpAmpGain_init;i<OpAmpGainMax;i++)
	  {
	    //Set the Gain of Leakage measure
	    amac[i]->write(AMACreg::OPAMP_GAIN_LEFT, i);
	    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	    
	    //Set Paramater for conversion
	    amac_icalib[i]->setParameter(AMAC_icalibrate::LEFT,BandGap,RampGain,j);
	    
	    //Read voltage value for Leakage measure
	    unsigned Leak_count = 0;
	    amac[i]->read(AMACreg::VALUE_LEFT_CH6, Leak_count);
	    std::this_thread::sleep_for(std::chrono::seconds(1));
		  
	    //Convert in current value
	    I_gain += amac_icalib[i]->Leakage_calibrate(Leak_count);
	  }
	
	//Do the mean of the leakage current
	double I_leak = I_gain/(OpAmpGainMax-OpAmpGain_init);
	
	logfile << ps.getVoltage()<<" "<<HV_supply<<" "<<I_leak<<" "<<Ileake_ADC;
	std::cout << I_leak<< "is the current"<<std::endl;
 
	/*switch(j)
	  {
	  case 1:
	    {
	    double HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB1);
	    double HV_supply =  HV_ADC*550/30;
	    double Ileake_ADC = HV_ADC/(30.0*pow(10.0,3.0));
	    uint8_t OpAmpGain_init;
	    uint8_t OpAmpGainMax;

	     //Read BandGapControl
	    unsigned BandGap;
	    amac1->read(AMACreg::BANDGAP_CONTROL,BandGap);

	    //Read RampGain
	    unsigned RampGain;
	    amac1->read(AMACreg::LEFT_RAMP_GAIN,RampGain);
	      if(Ileake_ADC < 20e-6)
		{
		  OpAmpGain_init = 0;
		  OpAmpGainMax = 2;
		}      
	      else if((Ileake_ADC >= 20e-6)&&(Ileake_ADC < 1e-3))
		{
		  OpAmpGain_init = 2;
		  OpAmpGainMax = 4;
		}
	      else if ((Ileake_ADC >= 1e-3)&&(Ileake_ADC < 7e-3))
		{
		  OpAmpGain_init = 4;
		  OpAmpGainMax = 16;
		}
	      else
		{
		  OpAmpGain_init = 0;
		  OpAmpGainMax = 16;
		  std::cout << "Measured Leakage current is not in the range" << std::endl;
		}
	      //Do a mean value of the measured leakage current
	      double I_gain = 0.0;
	      for(unsigned i=OpAmpGain_init;i<OpAmpGainMax;i++)
		{
		  //Set the Gain of Leakage measure
		  amac1->write(AMACreg::OPAMP_GAIN_LEFT, i);
		  std::this_thread::sleep_for(std::chrono::milliseconds(100));

		  //Set Paramater for conversion
		  amac1_icalib->setParameter(AMAC_icalibrate::LEFT,BandGap,RampGain,i);

		  //Read voltage value for Leakage measure
		  unsigned Leak_count = 0;
		  amac1->read(AMACreg::VALUE_LEFT_CH6, Leak_count);
		  std::this_thread::sleep_for(std::chrono::milliseconds(100));
		  
		  //Convert in current value
		  I_gain += amac1_icalib->Leakage_calibrate(Leak_count);
		}

	      //Do the mean of the leakage current
	      double I_leak = I_gain/(OpAmpGainMax-OpAmpGain_init);
	      
	      logfile << ps.getVoltage()<<" "<<HV_supply<<" "<<I_leak<<" "<<Ileake_ADC;
	    }
	      break;
	  case 2:
	    //m_adc_hv0->read(ADC_HVOUT_PB2);
	    break;
	  case 3:
	    {
	    double HV_ADC = m_adc_hv0->read(ADC_HVOUT_PB3);
	    double HV_supply =  HV_ADC*550/30;
	    double Ileake_ADC = HV_ADC/(30.0*pow(10.0,3.0));
	    uint8_t OpAmpGain_init;
	    uint8_t OpAmpGainMax;

	     //Read BandGapControl
	    unsigned BandGap;
	    amac3->read(AMACreg::BANDGAP_CONTROL,BandGap);

	    //Read RampGain
	    unsigned RampGain;
	    amac3->read(AMACreg::LEFT_RAMP_GAIN,RampGain);
	      if(Ileake_ADC < 20e-6)
		{
		  OpAmpGain_init = 0;
		  OpAmpGainMax = 2;
		}      
	      else if((Ileake_ADC >= 20e-6)&&(Ileake_ADC < 1e-3))
		{
		  OpAmpGain_init = 2;
		  OpAmpGainMax = 4;
		}
	      else if ((Ileake_ADC >= 1e-3)&&(Ileake_ADC < 7e-3))
		{
		  OpAmpGain_init = 4;
		  OpAmpGainMax = 16;
		}
	      else
		{
		  OpAmpGain_init = 0;
		  OpAmpGainMax = 16;
		  std::cout << "Measured Leakage current is not in the range" << std::endl;
		}
	      //Do a mean value of the measured leakage current
	      double I_gain = 0.0;
	      for(unsigned i=OpAmpGain_init;i<OpAmpGainMax;i++)
		{
		  //Set the Gain of Leakage measure
		  amac3->write(AMACreg::OPAMP_GAIN_LEFT, i);
		  std::this_thread::sleep_for(std::chrono::milliseconds(100));

		  //Set Paramater for conversion
		  amac3_icalib->setParameter(AMAC_icalibrate::LEFT,BandGap,RampGain,i);

		  //Read voltage value for Leakage measure
		  unsigned Leak_count = 0;
		  amac3->read(AMACreg::VALUE_LEFT_CH6, Leak_count);
		  std::this_thread::sleep_for(std::chrono::milliseconds(100));
		  
		  //Convert in current value
		  I_gain += amac3_icalib->Leakage_calibrate(Leak_count);
		}

	      //Do the mean of the leakage current
	      double I_leak = I_gain/(OpAmpGainMax-OpAmpGain_init);
	      
	      logfile << ps.getVoltage()<<" "<<HV_supply<<" "<<I_leak<<" "<<Ileake_ADC;
	    }
	    break;
	  case 4:
	    //return m_adc_hv0->read(ADC_HVOUT_PB4);
	    break;
	  case 5:
	    //return m_adc_hv1->read(ADC_HVOUT_PB5);
	    break;
	  case 6:
	    //return m_adc_hv1->read(ADC_HVOUT_PB6);
	    break;
	  case 7:
	    //return m_adc_hv1->read(ADC_HVOUT_PB7);
	    break;
	  case 8:
	    //return m_adc_hv1->read(ADC_HVOUT_PB8);
	    break;
	  case 9:
	    //return m_adc_hv2->read(ADC_HVOUT_PB9);
	    break;
	  default:
	    //return 0;
	    break;
	    }*/
      }
     logfile.close();
 }
  //*/
  // Power-off
#ifdef SCOPE
  pico.close();
#endif // SCOPE
  ps.turnOff();
  ps.setCh(1);
  ps.turnOff();

    return 0;

    clock_t end = clock();

    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    logpath = "log/" + TestName + "_Time.log";
    logfile.open(logpath, std::fstream::out);
    logfile << "testNum Time" << std::endl;
    logfile << t << " " << elapsed_secs << std::endl;

  }
}
