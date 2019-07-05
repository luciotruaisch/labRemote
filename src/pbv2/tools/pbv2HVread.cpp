#include <string>
#include <iostream>
#include <thread>

#include "PBv2TB.h"

int main(){
  
  PBv2TB tb;
  
  tb.setHVamp(0,1);
  
  std::this_thread::sleep_for(std::chrono::seconds(5));

  uint8_t Gain = 32;
  uint8_t PB = 32;
  tb.getHVamp(PB,Gain);
  std::cout << "Gain is:"<< (unsigned)Gain << "   Channel is:" << (unsigned)PB<< std::endl;

  while(1)
    {
      double HV = tb.getHVout();
      std::this_thread::sleep_for(std::chrono::seconds(5));
      std::cout << "HV measured:" << HV << std::endl;
    }

}
