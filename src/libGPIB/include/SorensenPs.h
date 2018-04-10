#ifndef SORENSENPS_H
#define SORENSENPS_H

// ####################
// Sorensen Power supply
// Author: Karol Krizka
// Date: April 2018
// Notes: Assuming Prologix GPIB to USB
// ###################

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "SerialCom.h"

class SorensenPs
{
public:
  SorensenPs(std::string dev, unsigned addr);
  ~SorensenPs();

  void init();
  void setCh(unsigned ch);
  void setVoltage(double volt);
  std::string getVoltage();
  void setCurrent(double cur);
  std::string getCurrent();
  void turnOn();
  void turnOff();

private:
  SerialCom *m_com;
  unsigned m_addr;
  unsigned m_ch =1;

  void send(std::string cmd);
  std::string receive(std::string cmd);
        
  std::chrono::milliseconds m_wait{200};
};

#endif
