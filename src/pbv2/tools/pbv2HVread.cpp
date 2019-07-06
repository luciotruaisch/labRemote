#include <string>
#include <iostream>
#include <thread>
#include <fstream>

#include "PBv2TB.h"
#include "SorensenPs.h"

int main(){
  
  PBv2TB tb;
  SorensenPs ps("/dev/ttyUSB0",11);
  ps.setCh(2);
  ps.init();
  ps.setCh(2);

  double Vmin = 0.1;
  double Vmax = 3.3;
  //Creat a test file
  std::ofstream TestFile;
  TestFile.open("HV_ADCread.csv");
  TestFile << "HVsupply,HVmes"<<std::endl;



  for(double V=0.0;V<Vmax;V+=Vmin)
    {
      ps.setVoltage(V);
      ps.turnOn();
      std::this_thread::sleep_for(std::chrono::seconds(2));
      double HV = tb.getHVout();
      double HV_supply = std::stod(ps.getVoltage());

      TestFile << HV_supply <<","<<HV<<std::endl;
      ps.turnOff();
    }
  TestFile.close();
}
