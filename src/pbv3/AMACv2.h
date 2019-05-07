#ifndef AMACv2_H_
#define AMACv2_H_

#include "AMACv2Reg.h"
#include "EndeavourCom.h"
#include "EndeavourRaw.h"

class AMACv2 : public EndeavourCom, public AMACv2Reg
{
public:
  AMACv2(unsigned short amacid, std::unique_ptr<EndeavourRaw> raw); // with hardware	
  ~AMACv2();

  void init();
  void initRegisters();

  virtual void write_reg(unsigned int address, unsigned int data);

  void wrField(AMACv2Field AMACv2Reg::* ref, uint32_t data);
  void wrField(const std::string& fieldName, uint32_t data);

  uint32_t rdField(AMACv2Field AMACv2Reg::* ref);
  uint32_t rdField(const std::string& fieldName);

  //
  // ADC configuration
  void setADCslope (double ADCslope);
  void setADCoffset(uint8_t ch, uint32_t counts);

  //
  // Calibrated ADC readings
  double getADC(uint8_t ch);

  double getVDDREG();
  double getAM();
  double getAM900();
  double getNTCx();
  double getNTCy();
  double getNTCpb();

  double getCur10V();
  double getCur1V();

private:
  double calibrateCounts(uint8_t ch, uint32_t counts);

  void syncReg(AMACv2Field AMACv2Reg::* ref);

  double   m_ADCslope      =1.;
  uint32_t m_ADCoffset[16] ={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

#endif // AMACv2_H_
