#include "PBv2TB.h"
#include "SorensenPs.h"

#include <iostream>
#include <cmath>

int main()
{
  PBv2TB tb;
  SorensenPs ps("/dev/ttyUSB0",11);

  ps.setCh(1);
  ps.init();
  ps.setVoltage(11.0);
  ps.setCurrent(5.0);
  ps.turnOn();
  
  double Vmin = 5;
  double Vmax = 60;

  double R1 = 520*pow(10.0,3.0);
  double R2 = 5.2*pow(10.0,3.0);

  double ratio = R2/(R1+R2);
  std::cout << "ratio is "<< ratio <<std::endl;

  ps.setCh(2);
  ps.setCurrent(0.1);

  //This part has to be commited if the amplifier check is not need
  tb.setHVamp(0,2);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  for(double V=0.0;V<Vmax;V+=Vmin)
    {
      ps.setVoltage(V);
      ps.turnOn();
      (tb.getPB(0))->write(AMACreg::HV_ENABLE,0x1);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      double Vex = (V+0.1)*ratio;
      std::cout << "Expected voltage is " << Vex*2 << std::endl;

      ps.turnOn();
      std::this_thread::sleep_for(std::chrono::seconds(5));
      (tb.getPB(0))->write(AMACreg::HV_ENABLE,0x1);
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      ps.turnOff();
    }
  ps.setCh(1);
  ps.turnOff();
  return 0;
}
