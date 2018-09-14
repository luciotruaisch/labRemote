#ifndef AMAC_H_
#define AMAC_H_

#include "AMACreg.h"
#include "EndeavourCom.h"
#include "EndeavourRaw.h"

class AMAC : public EndeavourCom, public AMACv2Reg
{
public:
  AMAC(unsigned short amacid, std::unique_ptr<EndeavourRaw> raw); // with hardware	
  ~AMAC();

  void init();

  virtual void write_reg(unsigned int address, unsigned int data);

  void wrField(AMACv2Field AMACv2Reg::* ref, uint32_t data);
  uint32_t rdField(AMACv2Field AMACv2Reg::* ref);

private:
  void syncReg(AMACv2Field AMACv2Reg::* ref);

};

#endif // AMAC_H_
