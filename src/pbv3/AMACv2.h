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

  virtual void write_reg(unsigned int address, unsigned int data);

  void wrField(AMACv2Field AMACv2Reg::* ref, uint32_t data);
  uint32_t rdField(AMACv2Field AMACv2Reg::* ref);

private:
  void syncReg(AMACv2Field AMACv2Reg::* ref);

};

#endif // AMACv2_H_
