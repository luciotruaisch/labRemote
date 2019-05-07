#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <fstream>
#include <string>

#include <sys/stat.h>
#include <dirent.h>

#ifdef FTDI
#include "FTDICom.h"
#endif

#include "Logger.h"
#include "MojoCom.h"
#include "I2CCom.h"
#include "AMAC.h"
#include "Bk85xx.h"
#include "AgilentPs.h"
#include "Keithley24XX.h"

loglevel_e loglevel = logINFO;

int main(int argc, char* argv[]) {
  //
  // Get settings from the command line
  if (argc < 4) {
    log(logERROR) << "Not enough parameters!";
    log(logERROR) << "Usage: " << argv[0] << " TESTNAME <Mojo/FTDI> <GPIB>";
    return -1;
  }

  std::string TestName = argv[1];
  std::string mojoDev = argv[2];
  std::string gpibDev = argv[3];

  //std::cout << "cout1" << std::endl;
#ifndef FTDI
  if(mojoDev=="FTDI")
    {
      log(logERROR) << "FTDI support not enabled.";
      return -1;
    }
#endif

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

  //std::cout << "cout2" << std::endl;
  //
  // Run tests
  log(logINFO) << "Initialising ...";
    
  //Set 11V and 9A on the power supply
  log(logINFO) << " ... Agilent PS:";
  AgilentPs ps(gpibDev, 10);
  ps.init();
  //  std::cout << "cout3" << std::endl;
  sleep(1);
  ps.setRange(20);
  //std::cout << "cout4" << std::endl;
  
  sleep(1);
  ps.setVoltage(11.0); //set to 11
  //std::cout << "cout5" << std::endl;
  sleep(1);
  ps.setCurrent(9.0); //set to 9
  //std::cout << "cout6" << std::endl;
  sleep(1);
  ps.turnOn();
  //  std::cout << "cout4" << std::endl;
  std::cout << "Trying to read the voltage" << std::endl;
  std::cout << ps.getVoltage() <<std::endl;
  std::cout << ps.getCurrent() <<std::endl;

  //Initiate the i2c communication
  std::unique_ptr<I2CCom> i2c;
#ifdef FTDI
  if(mojoDev=="FTDI")
    i2c.reset(new FTDICom());
  else
    i2c.reset(new MojoCom(mojoDev));
#else
  i2c.reset(new MojoCom(mojoDev));
#endif

  i2c->enableI2C();
  //Read I2C (Ch6)
  char data[2];
  data[0]=(uint)0;
  data[1]=0x0;
  //i2c->writeI2C(0x4A, 0x3c, data, 0);
  usleep(1000);
  //data[0]=0;
  //data[1]=0;
  std::string logpath;
  std::fstream logfile;
  
  logpath = "log/" + TestName + "_noamac.log";
  logfile.open(logpath, std::fstream::out);

  std::cout << "Time"<< " " << "data0_pb9" << " " <<"data1_pb9" <<  " " << "pb1" << " " << "pb2" << " " << "pb3" << " " << "pb4" << " " << "pb5" << " " << "pb6" << " " << "pb8" << " " << "pb" << " " << "PS_current" << " " << "PS_voltage" << " " << "temp" << " " << "humidity" << std::endl;
  logfile << "data0_pb9" << " " <<"data1_pb9" <<  " " << "pb1" << " " << "pb2" << " " << "pb3" << " " << "pb4" << " " << "pb5" << " " << "pb6" << " " << "pb8" << " " << "pb" << " " << "PS_current" << " " << "PS_voltage" << " " << "temp" << " " << "humidity" << std::endl;


  uint16_t bigdata;
  double adcread1=0.0,adcread2=0.0,adcread3=0.0,adcread4=0.0,adcread5=0.0,adcread6=0.0,adcread8=0.0,adcread9=0.0;
  
  char data1[4];
  unsigned int intdata=0;
  unsigned int status,humidata,tempdata;
  double humidity=0.0,temperature=0.0;

  while(1){
    std::time_t time = std::time(0);
    sleep(1);
    //address 0x3c for internal reference ON and 0x34 for OFF 
    
    //Read pb1
    i2c->readI2C(0x48, 0x4, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread1=3.3*bigdata/0xFFF;
    sleep(5);
    //Read pb2
    i2c->readI2C(0x48, 0x14, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread2=3.3*bigdata/0xFFF;
    //sleep(1);
    //Read pb3
    i2c->readI2C(0x48, 0x24, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread3=3.3*bigdata/0xFFF;
    //sleep(1);
    //Read pb4
    i2c->readI2C(0x48, 0x34, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread4=3.3*bigdata/0xFFF;
    //sleep(1);
    //Read pb5
    i2c->readI2C(0x4A, 0x4, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread5=3.3*bigdata/0xFFF;
    //sleep(1);
    //Read pb6
    i2c->readI2C(0x4A, 0x14, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread6=3.3*bigdata/0xFFF;
    //sleep(1);
    //Read pb8
    i2c->readI2C(0x4A, 0x24, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread8=3.3*bigdata/0xFFF;
    //sleep(1);
    //Read pb9
    i2c->readI2C(0x4A, 0x34, data, 2);
    bigdata=(data[0]<<8)|(data[1]<<0);
    adcread9=3.3*bigdata/0xFFF;
    //sleep(1);

    i2c->writeI2C(0x27,0x27,data1,4);
    usleep(500e3);
    i2c->readI2C(0x27,0x27,data1,4);
    
    for(uint i=0;i<4;i++){
      intdata<<=8;
      intdata|=(data1[i]&0xFF);
    }

    //Parse the data
    status=(intdata>>29)&0x3;
    humidata=(intdata>>16)&0x3FFF;
    tempdata=(intdata>> 2)&0x3FFF;
    //std::cout<< intdata << std::endl;
    humidity=((float)humidata)*100/(pow(2,14)-1.0);
    temperature=((((float)tempdata)*165.0)/(pow(2,14)-1.0))-40.0;
    
    std::cout << std::dec << time<< " "<< adcread1 << " " <<adcread2 << " " <<adcread3 << " " <<adcread4 << " " <<adcread5 << " " <<adcread6 << " " <<adcread8 << " " <<adcread9 << " " << ps.getCurrent() << " " << ps.getVoltage() << " "<< temperature << " " << humidity<<std::endl;
    logfile << std::dec << time << " "<< adcread1 << " " <<adcread2 << " " <<adcread3 << " " <<adcread4 << " " <<adcread5 << " " <<adcread6 << " " <<adcread8 << " " <<adcread9 << " " << ps.getCurrent() << " " << ps.getVoltage() << " "<< temperature << " " << humidity<< std::endl;

  }
 
  //Write output to the log file below
  //logfile <<"OTA_L" << " " << "OTA_R" << " " << "DVDD2" << " " << "BGO" << " " << "BGO_SM" << std::endl;
  //logfile <<ota_l <<" "<<ota_r<<" "<<dvdd2<<" "<<bgo<<" "<<bgo_sm1<<std::endl;
  
  logfile.close();

  ps.turnOff();
  return 0;
}
