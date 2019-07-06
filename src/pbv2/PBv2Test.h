#ifndef PBV2TEST_H
#define PBV2TEST_H

#include <memory>
#include <string>

#include "PBv2TB.h"
#include "SorensenPs.h"
#include "AMAC_icalibrate.h"

class PBv2Test
{
public:
  PBv2Test(const std::string& name, std::shared_ptr<PBv2TB> tb, uint8_t pbidx, std::shared_ptr<SorensenPs> ps);
  PBv2Test(const std::string& name, std::shared_ptr<PBv2TB> tb, uint8_t pbidx); 
  ~PBv2Test();

  bool runGeneral();
  bool runLVEnable();
  bool runHVEnable(double InHV);
  bool runDCDCEfficiency();
  bool runVin();
  bool runVinIn();
  bool runLeakage(std::string AMAC_ID);
  bool runLeakage(std::string AMAC_ID, double InHV);
  
private:
  std::string m_name;

  std::shared_ptr<PBv2TB> m_tb;
  uint8_t m_pbidx;
  std::shared_ptr<AMAC> m_pb;

  std::shared_ptr<SorensenPs> m_ps;
};

#endif // PBV2TEST_H
