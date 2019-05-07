#ifndef PBV3COMMPATCHSPICOM_H
#define PBV3COMMPATCHSPICOM_H

#include "SPICom.h"

class PBv3CommPatchSPICom : public SPICom
{
public:
  PBv3CommPatchSPICom(struct ftdi_context *ftdi, uint8_t csbit);
  virtual ~PBv3CommPatchSPICom();

  //
  // Write commands
  void write_reg32(uint32_t address, uint32_t data);
  void write_reg16(uint32_t address, uint16_t data);
  void write_reg8 (uint32_t address, uint8_t  data);

  void write_reg32(uint32_t data);
  void write_reg16(uint16_t data);
  void write_reg8 (uint8_t  data);

  void write_block(uint32_t address, const std::vector<uint8_t>& data);
  void write_block(const std::vector<uint8_t>& data);


  //
  // Read commands
  uint32_t read_reg32(uint32_t address);
  uint16_t read_reg16(uint32_t address);
  uint8_t  read_reg8 (uint32_t address);

  uint32_t read_reg32();
  uint16_t read_reg16();
  uint8_t  read_reg8 ();

  void read_block(uint32_t address, std::vector<uint8_t>& data);
  void read_block(std::vector<uint8_t>& data);

private:
  struct ftdi_context *m_ftdi;
  uint8_t m_csbit;
};

#endif // PBV3COMMPATCHSPICOM_H
