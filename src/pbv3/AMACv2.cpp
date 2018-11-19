#include "AMACv2.h"

#include <unistd.h>

AMACv2::AMACv2(unsigned short amacid, std::unique_ptr<EndeavourRaw> raw)
  : EndeavourCom(amacid, std::move(raw)), AMACv2Reg()
{ }

AMACv2::~AMACv2(){}

void AMACv2::init()
{
  // Set AMAC ID
  EndeavourCom::setid(EndeavourCom::REFMODE::IDPads, 0x0);
  usleep(10);

  // Read AMAC registers into memory
  uint data;
  for(uint reg=0;reg<AMACv2Reg::numRegs;reg++)
    {
      data=(reg==0)?read_reg(reg):readnext_reg();
      setReg(reg, data);
      usleep(10);
    }



  // for (auto const& p : regMap)
  //   { //set all amac registers according to default values
  //     if(canBeWritten(p.second))
  // 	syncReg(p.second);
  //   }  
}

void AMACv2::initRegisters(){
  // Setting which has given us 1 mV / count
  wrField(&AMACv2Reg::VDDbg  , 0xE);
  wrField(&AMACv2Reg::VDDbgen, 1);
  wrField(&AMACv2Reg::AMbg   , 0xE);
  wrField(&AMACv2Reg::AMbgen , 1);

  // Setting which give best offset for the Cur1(0)V current monitors
  wrField(&AMACv2::DCDCiOffset, 8);	

}

void AMACv2::syncReg(AMACv2Field AMACv2Reg::* ref)
{
  EndeavourCom::write_reg(getAddr(ref), (this->*ref).readRaw());
}

void AMACv2::wrField(AMACv2Field AMACv2Reg::* ref, uint32_t data)
{
  setField(ref, data);
  EndeavourCom::write_reg(getAddr(ref), (this->*ref).readRaw());
  usleep(10);
}

uint32_t AMACv2::rdField(AMACv2Field AMACv2Reg::* ref)
{
  uint32_t ret = EndeavourCom::read_reg(getAddr(ref));  
  setReg(getAddr(ref), ret);
  usleep(10);
  return getField(ref);
}

void AMACv2::write_reg(unsigned int address, unsigned int data)
{
  setReg(address, data);
  EndeavourCom::write_reg(address, data);
  usleep(10);
}
