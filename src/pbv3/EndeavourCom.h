#ifndef ENDEAVOURCOM_H
#define ENDEAVOURCOM_H

#include <memory>

#include "DeviceCom.h"
#include "EndeavourRaw.h"

class EndeavourCom : public DeviceCom
{
public:
  enum REFMODE {IDPads, EfuseId};

  EndeavourCom(unsigned short amacid, std::shared_ptr<DeviceCom> fpgaCom);

  const std::unique_ptr<EndeavourRaw>& raw();
  
  void enableSeqNum(bool enableSeqNum);

  void reset();

  void setid(REFMODE mode, unsigned int refid);

  virtual void write_reg(unsigned int address, unsigned int data);
  virtual unsigned int read_reg(unsigned int address);
  virtual unsigned int readnext_reg();
	
  //implemented in DeviceCom
  virtual void write_reg(std::vector<unsigned int> data_vec){};
  virtual void write_data(uint8_t data){};

  virtual void read_reg(unsigned int address, uint8_t* data, unsigned int len){};
  virtual void read_reg(uint8_t* data_in, uint8_t* data_out, unsigned int len){};

private:
  unsigned short m_amacid;

  bool m_enableSeqNum =false;
  unsigned short m_seqnum =0;

  std::unique_ptr<EndeavourRaw> m_raw;

  unsigned int calc_crc(unsigned long long int data) const;
};

#endif //ENDEAVOURCOM_H
