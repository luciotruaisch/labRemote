#include "LinPOL12VTestTools.h"

#include "Logger.h"

#include "EndeavourRawFTDI.h"
#include "EndeavourCom.h"
#include "EndeavourComException.h"

#include <memory>

namespace LinPOL12VTestTools {
  json testVOut_1L(GenericPs *ps, Bk85xx *load,  EndeavourRawFTDI *FTDI_dev, int curr_low, int curr_hi, int curr_step, int n_vals)
  {
    logger(logINFO) << "## testing V Out response " << LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now());

    std::cout<<"3V3 Reading Test In Func: "<<FTDI_dev->getADC()->read(0)<<std::endl;

    json testSum;
    testSum["testType"] = "STATUS";
    testSum["results"]["TIMESTART"] = LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = false;

    // try
    //   {
    // 	
    // 	load->turnOn();
    //   }
    // catch(std::string &s)
    //   {
    // 	logger(logERROR) << s;
    // 	testSum["error"] = s;
    // 	return testSum;
    //   }

   

    int index_outloop = 0;

    for(int curr_iter = curr_low; curr_iter <= curr_hi; curr_iter += curr_step){
    
      std::cout<< "Vin [V]\tIin [mA]\t 1V4 [mV]\t 3V3 [mV]\t I1V4 [mA]"<<std::endl;

      load->setCurrent(curr_iter);
      index_outloop =floor((curr_iter-curr_low)/curr_step);


      for (int index = 0; index<n_vals; index++){
      
	//Grab values
	double Iin = std::stod(ps->getCurrent());
	double Vin  = std::stod(ps->getVoltage());
	double IoneVfour = load->getValues().cur;
	double oneVfour_Vload = load->getValues().vol;//mV
	double oneVfour = FTDI_dev->getADC()->read(0);
	double threeVthree = FTDI_dev->getADC()->read(1);

   
	std::cout<< Vin <<"\t"<<Iin <<"\t"<<oneVfour<<"\t"<<threeVthree <<"\t"<<IoneVfour<<"\t"<<std::endl;
	testSum["results"]["VIN"             ][index+index_outloop*n_vals]=Vin;
	testSum["results"]["IIN"             ][index+index_outloop*n_vals]=Iin;
	testSum["results"]["oneVfour_Vload"     ][index+index_outloop*n_vals]=oneVfour_Vload*1e-3;
	testSum["results"]["oneVfour"     ][index+index_outloop*n_vals]=oneVfour;
	testSum["results"]["threeVthree"  ][index+index_outloop*n_vals]=threeVthree;
	testSum["results"]["IoneVfour"       ][index+index_outloop*n_vals]=IoneVfour*1e-3;
	
	
	usleep(20);
      }
    }    
    testSum["results"]["TIMEEND"] = LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = true;
    return testSum;
  }
  
 std::string getTimeAsString(std::chrono::system_clock::time_point t)
  {
    auto as_time_t = std::chrono::system_clock::to_time_t(t);
    struct tm tm;
    char some_buffer[128];
    if (::gmtime_r(&as_time_t, &tm))
      if (std::strftime(some_buffer, sizeof(some_buffer), "%Y_%m_%d-%X", &tm))
	return std::string(some_buffer);
    throw std::runtime_error("Failed to get current date as string");
  }
  
  json testVOut_2L(GenericPs *ps, Bk85xx *load, GenericLoad *load2, int curr_low1, int curr_hi1,int curr_low2, int curr_hi2 , int curr_step1, int curr_step2, int n_vals)
  {
    logger(logINFO) << "## testing V Out response " << LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now());
    
       
    json testSum;
    testSum["testType"] = "STATUS";
    testSum["results"]["TIMESTART"] = LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = false;

    // try
    //   {
    // 	
    // 	load->turnOn();
    //   }
    // catch(std::string &s)
    //   {
    // 	logger(logERROR) << s;
    // 	testSum["error"] = s;
    // 	return testSum;
    //   }

   

    int index_outloop = 0;

    for(int curr_iter1 = curr_low1; curr_iter1 <= curr_hi1; curr_iter1 += curr_step1){
      load->setCurrent(curr_iter1);

      for(int curr_iter2 = curr_low2; curr_iter2 <= curr_hi2; curr_iter2 += curr_step2){
    
	std::cout<< "Vin [V]\t Iin [mA]\t 1V4 [mV]\t 3V3 [mV]\t I1V4 [mA] \t I3V3 [A]"<<std::endl;
	
	load2->setCurrent(curr_iter2/1000.);
	
	

	for (int index = 0; index<n_vals; index++){

      
	  //Grab values
	  double Iin = std::stod(ps->getCurrent());
	  double Vin  = std::stod(ps->getVoltage());
	  double IoneVfour = load->getValues().cur;
	  double oneVfour = load->getValues().vol;
	  double threeVthree = std::stod(load2->getVoltage());  
	  double IthreeVthree = std::stod(load2->getCurrent()); 

   
	  std::cout<< Vin <<"\t "<<Iin <<"\t "<<oneVfour<<"\t "<<threeVthree <<"\t "<<IoneVfour<<"\t "<<IthreeVthree<<std::endl;
	  testSum["results"]["VIN"             ][index_outloop]=Vin;
	  testSum["results"]["IIN"             ][index_outloop]=Iin;
	  testSum["results"]["oneVfour"     ][index_outloop]=oneVfour;
	  testSum["results"]["threeVthree"  ][index_outloop]=threeVthree;
	  testSum["results"]["IthreeVthree"  ][index_outloop]= IthreeVthree*1e-3;
	  testSum["results"]["IoneVfour"       ][index_outloop]=IoneVfour*1e-3;
	
	  index_outloop++;
	
	  usleep(20);
	}      
      }
    }    
    testSum["results"]["TIMEEND"] = LinPOL12VTestTools::getTimeAsString(std::chrono::system_clock::now());
    testSum["passed"] = true;
    return testSum;
  }
  
}

