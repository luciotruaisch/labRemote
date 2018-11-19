#include "AMACv2Reg.h"

AMACv2Field::AMACv2Field()
{ }

bool AMACv2Field::canBeWrittenField() const
{ return (m_rw != WO); }

void AMACv2Field::initReg(uint32_t* cfg, rw_t rw, uint32_t defaultVal, uint8_t width, uint8_t offset, uint8_t regNbr, const std::string& fieldName)
{
  m_cfg = cfg;
  m_rw = rw;
  m_defaultVal = defaultVal;
  m_width = width;
  m_offset = offset;
  m_regNbr = regNbr;
  m_mask = (uint32_t) (((1 << m_width) - 1) << m_offset);
  m_fieldName = fieldName;
  if(m_rw != RO)
    this->write(m_defaultVal);
}

void AMACv2Field::write(const uint32_t& cfgBits)
{
  if(m_rw == RO)
    {
      std::cerr << " --> Error: Read-only register \"" << m_fieldName <<"\""<< std::endl;
      return;
    }
  m_cfg[m_regNbr] = (m_cfg[m_regNbr]&~m_mask) | ((cfgBits << m_offset) & m_mask);
}

uint32_t AMACv2Field::read() const
{
  if(m_rw == WO)
    {
      std::cerr << " --> Error: Write-only register \"" << m_fieldName <<"\""<< std::endl;
      return 0;
    }
  return ((m_cfg[m_regNbr] & m_mask) >> m_offset);
}

uint32_t AMACv2Field::readRaw() const
{
  if(m_rw == WO)
    {
      std::cerr << " --> Error: Write-only register \"" << m_fieldName <<"\""<< std::endl;
      return 0;
    }
  return m_cfg[m_regNbr];
}

void AMACv2Field::writeRaw(const uint32_t& cfgBits)
{
  if(m_rw == RO)
    {
      std::cerr << " --> Error: Read-only register \"" << m_fieldName <<"\""<< std::endl;
      return;
    }
  m_cfg[m_regNbr] = cfgBits;
}

uint8_t AMACv2Field::addr() const
{ return m_regNbr; }


AMACv2Reg::AMACv2Reg()
{
  init();
}

void AMACv2Reg::init()
{
	
  for (unsigned int i=0; i<numRegs; i++) m_cfg[i] = 0;
	
  StatusAM  .initReg(m_cfg, RO, 0, 1, 31, 0, "StatusAM"  ); regMap["StatusAM"  ] = &AMACv2Reg::StatusAM;
  StatusWARN.initReg(m_cfg, RO, 0, 1, 29, 0, "StatusWARN"); regMap["StatusWARN"] = &AMACv2Reg::StatusWARN;
  StatusDCDC.initReg(m_cfg, RO, 0, 1, 28, 0, "StatusDCDC"); regMap["StatusDCDC"] = &AMACv2Reg::StatusDCDC;
  StatusHV3.initReg(m_cfg, RO, 0, 1, 27, 0, "StatusHV3"); regMap["StatusHV3"] = &AMACv2Reg::StatusHV3;
  StatusHV2.initReg(m_cfg, RO, 0, 1, 26, 0, "StatusHV2"); regMap["StatusHV2"] = &AMACv2Reg::StatusHV2;
  StatusHV1.initReg(m_cfg, RO, 0, 1, 25, 0, "StatusHV1"); regMap["StatusHV1"] = &AMACv2Reg::StatusHV1;
  StatusHV0.initReg(m_cfg, RO, 0, 1, 24, 0, "StatusHV0"); regMap["StatusHV0"] = &AMACv2Reg::StatusHV0;
  StatusY2LDO.initReg(m_cfg, RO, 0, 1, 22, 0, "StatusY2LDO"); regMap["StatusY2LDO"] = &AMACv2Reg::StatusY2LDO;
  StatusY1LDO.initReg(m_cfg, RO, 0, 1, 21, 0, "StatusY1LDO"); regMap["StatusY1LDO"] = &AMACv2Reg::StatusY1LDO;
  StatusY0LDO.initReg(m_cfg, RO, 0, 1, 20, 0, "StatusY0LDO"); regMap["StatusY0LDO"] = &AMACv2Reg::StatusY0LDO;
  StatusX2LDO.initReg(m_cfg, RO, 0, 1, 18, 0, "StatusX2LDO"); regMap["StatusX2LDO"] = &AMACv2Reg::StatusX2LDO;
  StatusX1LDO.initReg(m_cfg, RO, 0, 1, 17, 0, "StatusX1LDO"); regMap["StatusX1LDO"] = &AMACv2Reg::StatusX1LDO;
  StatusX0LDO.initReg(m_cfg, RO, 0, 1, 16, 0, "StatusX0LDO"); regMap["StatusX0LDO"] = &AMACv2Reg::StatusX0LDO;
  StatusGPI.initReg(m_cfg, RO, 0, 1, 12, 0, "StatusGPI"); regMap["StatusGPI"] = &AMACv2Reg::StatusGPI;
  StatusPGOOD.initReg(m_cfg, RO, 0, 1, 8, 0, "StatusPGOOD"); regMap["StatusPGOOD"] = &AMACv2Reg::StatusPGOOD;
  StatusILockWARN.initReg(m_cfg, RO, 0, 1, 5, 0, "StatusILockWARN"); regMap["StatusILockWARN"] = &AMACv2Reg::StatusILockWARN;
  StatusILockDCDC.initReg(m_cfg, RO, 0, 1, 4, 0, "StatusILockDCDC"); regMap["StatusILockDCDC"] = &AMACv2Reg::StatusILockDCDC;
  StatusILockHV2.initReg(m_cfg, RO, 0, 1, 3, 0, "StatusILockHV2"); regMap["StatusILockHV2"] = &AMACv2Reg::StatusILockHV2;
  StatusILockHV2.initReg(m_cfg, RO, 0, 1, 2, 0, "StatusILockHV2"); regMap["StatusILockHV2"] = &AMACv2Reg::StatusILockHV2;
  StatusILockYLDO.initReg(m_cfg, RO, 0, 1, 1, 0, "StatusILockYLDO"); regMap["StatusILockYLDO"] = &AMACv2Reg::StatusILockYLDO;
  StatusILockxLDO.initReg(m_cfg, RO, 0, 1, 0, 0, "StatusILockxLDO"); regMap["StatusILockxLDO"] = &AMACv2Reg::StatusILockxLDO;
  // 1 - HxFlags
  HxFlagsHi.initReg(m_cfg, RO, 0, 16, 16, 1, "HxFlagsHi"); regMap["HxFlagsHi"] = &AMACv2Reg::HxFlagsHi;
  HxFlagsLo.initReg(m_cfg, RO, 0, 16, 0, 1, "HxFlagsLo"); regMap["HxFlagsLo"] = &AMACv2Reg::HxFlagsLo;
  // 2 - HyFlags
  HyFlagsHi.initReg(m_cfg, RO, 0, 16, 16, 2, "HyFlagsHi"); regMap["HyFlagsHi"] = &AMACv2Reg::HyFlagsHi;
  HyFlagsLo.initReg(m_cfg, RO, 0, 16, 0, 2, "HyFlagsLo"); regMap["HyFlagsLo"] = &AMACv2Reg::HyFlagsLo;
  // 3 - HV0Flags
  HV0FlagsHi.initReg(m_cfg, RO, 0, 16, 16, 3, "HV0FlagsHi"); regMap["HV0FlagsHi"] = &AMACv2Reg::HV0FlagsHi;
  HV0FlagsLo.initReg(m_cfg, RO, 0, 16, 0, 3, "HV0FlagsLo"); regMap["HV0FlagsLo"] = &AMACv2Reg::HV0FlagsLo;
  // 4 - HV2Flags
  HV2FlagsHi.initReg(m_cfg, RO, 0, 16, 16, 4, "HV2FlagsHi"); regMap["HV2FlagsHi"] = &AMACv2Reg::HV2FlagsHi;
  HV2FlagsLo.initReg(m_cfg, RO, 0, 16, 0, 4, "HV2FlagsLo"); regMap["HV2FlagsLo"] = &AMACv2Reg::HV2FlagsLo;
  // 5 - DCDCflags
  DCDCflagsHi.initReg(m_cfg, RO, 0, 16, 16, 5, "DCDCflagsHi"); regMap["DCDCflagsHi"] = &AMACv2Reg::DCDCflagsHi;
  DCDCflagsLo.initReg(m_cfg, RO, 0, 16, 0, 5, "DCDCflagsLo"); regMap["DCDCflagsLo"] = &AMACv2Reg::DCDCflagsLo;
  // 6 - WRNflags
  WRNflagsHi.initReg(m_cfg, RO, 0, 16, 16, 6, "WRNflagsHi"); regMap["WRNflagsHi"] = &AMACv2Reg::WRNflagsHi;
  WRNflagsLo.initReg(m_cfg, RO, 0, 16, 0, 6, "WRNflagsLo"); regMap["WRNflagsLo"] = &AMACv2Reg::WRNflagsLo;
  // 7 - SynFlags
  SynFlagsWRN.initReg(m_cfg, RO, 0, 2, 20, 7, "SynFlagsWRN"); regMap["SynFlagsWRN"] = &AMACv2Reg::SynFlagsWRN;
  SynFlagsDCDC.initReg(m_cfg, RO, 0, 2, 16, 7, "SynFlagsDCDC"); regMap["SynFlagsDCDC"] = &AMACv2Reg::SynFlagsDCDC;
  SynFlagsHV2.initReg(m_cfg, RO, 0, 2, 12, 7, "SynFlagsHV2"); regMap["SynFlagsHV2"] = &AMACv2Reg::SynFlagsHV2;
  SynFlagsHV2.initReg(m_cfg, RO, 0, 2, 8, 7, "SynFlagsHV2"); regMap["SynFlagsHV2"] = &AMACv2Reg::SynFlagsHV2;
  SynFlagsHy.initReg(m_cfg, RO, 0, 2, 4, 7, "SynFlagsHy"); regMap["SynFlagsHy"] = &AMACv2Reg::SynFlagsHy;
  SynFlagsHx.initReg(m_cfg, RO, 0, 2, 0, 7, "SynFlagsHx"); regMap["SynFlagsHx"] = &AMACv2Reg::SynFlagsHx;
  // 10 - Value0
  Value0AMen.initReg(m_cfg, RO, 0, 1, 31, 10, "Value0AMen"); regMap["Value0AMen"] = &AMACv2Reg::Value0AMen;
  Ch0Value.initReg(m_cfg, RO, 0, 10, 0, 10, "Ch0Value"); regMap["Ch0Value"] = &AMACv2Reg::Ch0Value;
  Ch1Value.initReg(m_cfg, RO, 0, 10, 10, 10, "Ch1Value"); regMap["Ch1Value"] = &AMACv2Reg::Ch1Value;
  Ch2Value.initReg(m_cfg, RO, 0, 10, 20, 10, "Ch2Value"); regMap["Ch2Value"] = &AMACv2Reg::Ch2Value;
  // 11 - Value1
  Value1AMen.initReg(m_cfg, RO, 0, 1, 31, 11, "Value1AMen"); regMap["Value1AMen"] = &AMACv2Reg::Value1AMen;
  Ch3Value.initReg(m_cfg, RO, 0, 10, 0, 11, "Ch3Value"); regMap["Ch3Value"] = &AMACv2Reg::Ch3Value;
  Ch4Value.initReg(m_cfg, RO, 0, 10, 10, 11, "Ch4Value"); regMap["Ch4Value"] = &AMACv2Reg::Ch4Value;
  Ch5Value.initReg(m_cfg, RO, 0, 10, 20, 11, "Ch5Value"); regMap["Ch5Value"] = &AMACv2Reg::Ch5Value;
  // 12 - Value2
  Value2AMen.initReg(m_cfg, RO, 0, 1, 31, 12, "Value2AMen"); regMap["Value2AMen"] = &AMACv2Reg::Value2AMen;
  Ch6Value.initReg(m_cfg, RO, 0, 10, 0, 12, "Ch6Value"); regMap["Ch6Value"] = &AMACv2Reg::Ch6Value;
  Ch7Value.initReg(m_cfg, RO, 0, 10, 10, 12, "Ch7Value"); regMap["Ch7Value"] = &AMACv2Reg::Ch7Value;
  Ch8Value.initReg(m_cfg, RO, 0, 10, 20, 12, "Ch8Value"); regMap["Ch8Value"] = &AMACv2Reg::Ch8Value;
  // 13 - Value3
  Value3AMen.initReg(m_cfg, RO, 0, 1, 31, 13, "Value3AMen"); regMap["Value3AMen"] = &AMACv2Reg::Value3AMen;
  Ch9Value.initReg(m_cfg, RO, 0, 10, 0, 13, "Ch9Value"); regMap["Ch9Value"] = &AMACv2Reg::Ch9Value;
  Ch10Value.initReg(m_cfg, RO, 0, 10, 10, 13, "Ch10Value"); regMap["Ch10Value"] = &AMACv2Reg::Ch10Value;
  Ch11Value.initReg(m_cfg, RO, 0, 10, 20, 13, "Ch11Value"); regMap["Ch11Value"] = &AMACv2Reg::Ch11Value;
  // 14 - Value4
  Value4AMen.initReg(m_cfg, RO, 0, 1, 31, 14, "Value4AMen"); regMap["Value4AMen"] = &AMACv2Reg::Value4AMen;
  Ch12Value.initReg(m_cfg, RO, 0, 10, 0, 14, "Ch12Value"); regMap["Ch12Value"] = &AMACv2Reg::Ch12Value;
  Ch13Value.initReg(m_cfg, RO, 0, 10, 10, 14, "Ch13Value"); regMap["Ch13Value"] = &AMACv2Reg::Ch13Value;
  Ch14Value.initReg(m_cfg, RO, 0, 10, 20, 14, "Ch14Value"); regMap["Ch14Value"] = &AMACv2Reg::Ch14Value;
  // 15 - Value5
  Value5AMen.initReg(m_cfg, RO, 0, 1, 31, 15, "Value5AMen"); regMap["Value5AMen"] = &AMACv2Reg::Value5AMen;
  Ch15Value.initReg(m_cfg, RO, 0, 10, 0, 15, "Ch15Value"); regMap["Ch15Value"] = &AMACv2Reg::Ch15Value;
  // 31 - SerNum
  PadID.initReg(m_cfg, RO, 0, 5, 24, 31, "PadID"); regMap["PadID"] = &AMACv2Reg::PadID;
  SerNum.initReg(m_cfg, RO, 0, 16, 0, 31, "SerNum"); regMap["SerNum"] = &AMACv2Reg::SerNum;
  // 32 - FlagResets
  FlagResetWRN.initReg(m_cfg, WO, 0, 1, 5, 32, "FlagResetWRN"); regMap["FlagResetWRN"] = &AMACv2Reg::FlagResetWRN;
  FlagResetDCDC.initReg(m_cfg, WO, 0, 1, 4, 32, "FlagResetDCDC"); regMap["FlagResetDCDC"] = &AMACv2Reg::FlagResetDCDC;
  FlagResetHV2.initReg(m_cfg, WO, 0, 1, 3, 32, "FlagResetHV2"); regMap["FlagResetHV2"] = &AMACv2Reg::FlagResetHV2;
  FlagResetHV0.initReg(m_cfg, WO, 0, 1, 2, 32, "FlagResetHV0"); regMap["FlagResetHV0"] = &AMACv2Reg::FlagResetHV0;
  FlagResetXLDO.initReg(m_cfg, WO, 0, 1, 1, 32, "FlagResetXLDO"); regMap["FlagResetXLDO"] = &AMACv2Reg::FlagResetXLDO;
  FlagResetYLDO.initReg(m_cfg, WO, 0, 1, 0, 32, "FlagResetYLDO"); regMap["FlagResetYLDO"] = &AMACv2Reg::FlagResetYLDO;
  // 33 - LogicReset
  LogicReset.initReg(m_cfg, WO, 0, 32, 0, 33, "LogicReset"); regMap["LogicReset"] = &AMACv2Reg::LogicReset;
  // 34 - HardReset
  HardReset.initReg(m_cfg, WO, 0, 32, 0, 34, "HardReset"); regMap["HardReset"] = &AMACv2Reg::HardReset;
  // 40 - CntSet
  CntSetHV3frq.initReg(m_cfg, RW, 3, 2, 29, 40, "CntSetHV3frq"); regMap["CntSetHV3frq"] = &AMACv2Reg::CntSetHV3frq;
  CntSetHV3en.initReg(m_cfg, RW, 0, 1, 28, 40, "CntSetHV3en"); regMap["CntSetHV3en"] = &AMACv2Reg::CntSetHV3en;
  CntSetHV2frq.initReg(m_cfg, RW, 3, 2, 25, 40, "CntSetHV2frq"); regMap["CntSetHV2frq"] = &AMACv2Reg::CntSetHV2frq;
  CntSetHV2en.initReg(m_cfg, RW, 0, 1, 24, 40, "CntSetHV2en"); regMap["CntSetHV2en"] = &AMACv2Reg::CntSetHV2en;
  CntSetHV1frq.initReg(m_cfg, RW, 3, 2, 21, 40, "CntSetHV1frq"); regMap["CntSetHV1frq"] = &AMACv2Reg::CntSetHV1frq;
  CntSetHV1en.initReg(m_cfg, RW, 0, 1, 20, 40, "CntSetHV1en"); regMap["CntSetHV1en"] = &AMACv2Reg::CntSetHV1en;
  CntSetHV0frq.initReg(m_cfg, RW, 3, 2, 17, 40, "CntSetHV0frq"); regMap["CntSetHV0frq"] = &AMACv2Reg::CntSetHV0frq;
  CntSetHV0en.initReg(m_cfg, RW, 0, 1, 16, 40, "CntSetHV0en"); regMap["CntSetHV0en"] = &AMACv2Reg::CntSetHV0en;
  CntSetHyLDO2en.initReg(m_cfg, RW, 0, 1, 14, 40, "CntSetHyLDO2en"); regMap["CntSetHyLDO2en"] = &AMACv2Reg::CntSetHyLDO2en;
  CntSetHyLDO1en.initReg(m_cfg, RW, 0, 1, 13, 40, "CntSetHyLDO1en"); regMap["CntSetHyLDO1en"] = &AMACv2Reg::CntSetHyLDO1en;
  CntSetHyLDO0en.initReg(m_cfg, RW, 0, 1, 12, 40, "CntSetHyLDO0en"); regMap["CntSetHyLDO0en"] = &AMACv2Reg::CntSetHyLDO0en;
  CntSetHxLDO2en.initReg(m_cfg, RW, 0, 1, 10, 40, "CntSetHxLDO2en"); regMap["CntSetHxLDO2en"] = &AMACv2Reg::CntSetHxLDO2en;
  CntSetHxLDO1en.initReg(m_cfg, RW, 0, 1, 9, 40, "CntSetHxLDO1en"); regMap["CntSetHxLDO1en"] = &AMACv2Reg::CntSetHxLDO1en;
  CntSetHxLDO0en.initReg(m_cfg, RW, 0, 1, 8, 40, "CntSetHxLDO0en"); regMap["CntSetHxLDO0en"] = &AMACv2Reg::CntSetHxLDO0en;
  CntSetWARN.initReg(m_cfg, RW, 0, 1, 4, 40, "CntSetWARN"); regMap["CntSetWARN"] = &AMACv2Reg::CntSetWARN;
  // 41 - CntSetC
  CntSetCHV3frq.initReg(m_cfg, RW, 3, 2, 29, 41, "CntSetCHV3frq"); regMap["CntSetCHV3frq"] = &AMACv2Reg::CntSetCHV3frq;
  CntSetCHV3en.initReg(m_cfg, RW, 0, 1, 28, 41, "CntSetCHV3en"); regMap["CntSetCHV3en"] = &AMACv2Reg::CntSetCHV3en;
  CntSetCHV2frq.initReg(m_cfg, RW, 3, 2, 25, 41, "CntSetCHV2frq"); regMap["CntSetCHV2frq"] = &AMACv2Reg::CntSetCHV2frq;
  CntSetCHV2en.initReg(m_cfg, RW, 0, 1, 24, 41, "CntSetCHV2en"); regMap["CntSetCHV2en"] = &AMACv2Reg::CntSetCHV2en;
  CntSetCHV1frq.initReg(m_cfg, RW, 3, 2, 21, 41, "CntSetCHV1frq"); regMap["CntSetCHV1frq"] = &AMACv2Reg::CntSetCHV1frq;
  CntSetCHV1en.initReg(m_cfg, RW, 0, 1, 20, 41, "CntSetCHV1en"); regMap["CntSetCHV1en"] = &AMACv2Reg::CntSetCHV1en;
  CntSetCHV0frq.initReg(m_cfg, RW, 3, 2, 17, 41, "CntSetCHV0frq"); regMap["CntSetCHV0frq"] = &AMACv2Reg::CntSetCHV0frq;
  CntSetCHV0en.initReg(m_cfg, RW, 0, 1, 16, 41, "CntSetCHV0en"); regMap["CntSetCHV0en"] = &AMACv2Reg::CntSetCHV0en;
  CntSetCHyLDO2en.initReg(m_cfg, RW, 0, 1, 14, 41, "CntSetCHyLDO2en"); regMap["CntSetCHyLDO2en"] = &AMACv2Reg::CntSetCHyLDO2en;
  CntSetCHyLDO1en.initReg(m_cfg, RW, 0, 1, 13, 41, "CntSetCHyLDO1en"); regMap["CntSetCHyLDO1en"] = &AMACv2Reg::CntSetCHyLDO1en;
  CntSetCHyLDO0en.initReg(m_cfg, RW, 0, 1, 12, 41, "CntSetCHyLDO0en"); regMap["CntSetCHyLDO0en"] = &AMACv2Reg::CntSetCHyLDO0en;
  CntSetCHxLDO2en.initReg(m_cfg, RW, 0, 1, 10, 41, "CntSetCHxLDO2en"); regMap["CntSetCHxLDO2en"] = &AMACv2Reg::CntSetCHxLDO2en;
  CntSetCHxLDO1en.initReg(m_cfg, RW, 0, 1, 9, 41, "CntSetCHxLDO1en"); regMap["CntSetCHxLDO1en"] = &AMACv2Reg::CntSetCHxLDO1en;
  CntSetCHxLDO0en.initReg(m_cfg, RW, 0, 1, 8, 41, "CntSetCHxLDO0en"); regMap["CntSetCHxLDO0en"] = &AMACv2Reg::CntSetCHxLDO0en;
  CntSetCWARN.initReg(m_cfg, RW, 0, 1, 4, 41, "CntSetCWARN"); regMap["CntSetCWARN"] = &AMACv2Reg::CntSetCWARN;
  // 42 - DCDCen
  DCDCAdj.initReg(m_cfg, RW, 0, 2, 4, 42, "DCDCAdj"); regMap["DCDCAdj"] = &AMACv2Reg::DCDCAdj;
  DCDCen.initReg(m_cfg, RW, 0, 1, 0, 42, "DCDCen"); regMap["DCDCen"] = &AMACv2Reg::DCDCen;
  // 43 - DCDCenC
  DCDCAdjC.initReg(m_cfg, RW, 0, 2, 4, 43, "DCDCAdjC"); regMap["DCDCAdjC"] = &AMACv2Reg::DCDCAdjC;
  DCDCenC.initReg(m_cfg, RW, 0, 1, 0, 43, "DCDCenC"); regMap["DCDCenC"] = &AMACv2Reg::DCDCenC;
  // 44 - Ilock
  IlockWRN.initReg(m_cfg, RW, 0, 1, 5, 44, "IlockWRN"); regMap["IlockWRN"] = &AMACv2Reg::IlockWRN;
  IlockDCDC.initReg(m_cfg, RW, 0, 1, 4, 44, "IlockDCDC"); regMap["IlockDCDC"] = &AMACv2Reg::IlockDCDC;
  IlockHV2.initReg(m_cfg, RW, 0, 1, 3, 44, "IlockHV2"); regMap["IlockHV2"] = &AMACv2Reg::IlockHV2;
  IlockHV2.initReg(m_cfg, RW, 0, 1, 2, 44, "IlockHV2"); regMap["IlockHV2"] = &AMACv2Reg::IlockHV2;
  IlockHy.initReg(m_cfg, RW, 0, 1, 1, 44, "IlockHy"); regMap["IlockHy"] = &AMACv2Reg::IlockHy;
  IlockHx.initReg(m_cfg, RW, 0, 1, 0, 44, "IlockHx"); regMap["IlockHx"] = &AMACv2Reg::IlockHx;
  // 45 - IlockC
  IlockCWRN.initReg(m_cfg, RW, 0, 1, 5, 45, "IlockCWRN"); regMap["IlockCWRN"] = &AMACv2Reg::IlockCWRN;
  IlockCDCDC.initReg(m_cfg, RW, 0, 1, 4, 45, "IlockCDCDC"); regMap["IlockCDCDC"] = &AMACv2Reg::IlockCDCDC;
  IlockCHV2.initReg(m_cfg, RW, 0, 1, 3, 45, "IlockCHV2"); regMap["IlockCHV2"] = &AMACv2Reg::IlockCHV2;
  IlockCHV2.initReg(m_cfg, RW, 0, 1, 2, 45, "IlockCHV2"); regMap["IlockCHV2"] = &AMACv2Reg::IlockCHV2;
  IlockCHy.initReg(m_cfg, RW, 0, 1, 1, 45, "IlockCHy"); regMap["IlockCHy"] = &AMACv2Reg::IlockCHy;
  IlockCHx.initReg(m_cfg, RW, 0, 1, 0, 45, "IlockCHx"); regMap["IlockCHx"] = &AMACv2Reg::IlockCHx;
  // 46 - RstCnt
  RstCntHyHCCresetB.initReg(m_cfg, RW, 0, 1, 16, 46, "RstCntHyHCCresetB"); regMap["RstCntHyHCCresetB"] = &AMACv2Reg::RstCntHyHCCresetB;
  RstCntHxHCCresetB.initReg(m_cfg, RW, 0, 1, 8, 46, "RstCntHxHCCresetB"); regMap["RstCntHxHCCresetB"] = &AMACv2Reg::RstCntHxHCCresetB;
  RstCntOF.initReg(m_cfg, RW, 0, 1, 0, 46, "RstCntOF"); regMap["RstCntOF"] = &AMACv2Reg::RstCntOF;
  // 47 - RstCntC
  RstCntCHyHCCresetB.initReg(m_cfg, RW, 0, 1, 16, 47, "RstCntCHyHCCresetB"); regMap["RstCntCHyHCCresetB"] = &AMACv2Reg::RstCntCHyHCCresetB;
  RstCntCHxHCCresetB.initReg(m_cfg, RW, 0, 1, 8, 47, "RstCntCHxHCCresetB"); regMap["RstCntCHxHCCresetB"] = &AMACv2Reg::RstCntCHxHCCresetB;
  RstCntCOF.initReg(m_cfg, RW, 0, 1, 0, 47, "RstCntCOF"); regMap["RstCntCOF"] = &AMACv2Reg::RstCntCOF;
  // 48 - AMen
  AMzeroCalib.initReg(m_cfg, RW, 0, 1, 8, 48, "AMzeroCalib"); regMap["AMzeroCalib"] = &AMACv2Reg::AMzeroCalib;
  AMen.initReg(m_cfg, RW, 1, 1, 0, 48, "AMen"); regMap["AMen"] = &AMACv2Reg::AMen;
  // 49 - AMenC
  AMzeroCalibC.initReg(m_cfg, RW, 0, 1, 8, 49, "AMzeroCalibC"); regMap["AMzeroCalibC"] = &AMACv2Reg::AMzeroCalibC;
  AMenC.initReg(m_cfg, RW, 1, 1, 0, 49, "AMenC"); regMap["AMenC"] = &AMACv2Reg::AMenC;
  // 50 - AMpwr
  ReqDCDCPGOOD.initReg(m_cfg, RW, 1, 1, 8, 50, "ReqDCDCPGOOD"); regMap["ReqDCDCPGOOD"] = &AMACv2Reg::ReqDCDCPGOOD;
  DCDCenToPwrAMAC.initReg(m_cfg, RW, 0, 1, 0, 50, "DCDCenToPwrAMAC"); regMap["DCDCenToPwrAMAC"] = &AMACv2Reg::DCDCenToPwrAMAC;
  // 51 - AMpwrC
  ReqDCDCPGOODC.initReg(m_cfg, RW, 1, 1, 8, 51, "ReqDCDCPGOODC"); regMap["ReqDCDCPGOODC"] = &AMACv2Reg::ReqDCDCPGOODC;
  DCDCenToPwrAMACC.initReg(m_cfg, RW, 0, 1, 0, 51, "DCDCenToPwrAMACC"); regMap["DCDCenToPwrAMACC"] = &AMACv2Reg::DCDCenToPwrAMACC;
  // 52 - BgCnt
  AMbgen .initReg(m_cfg, RW, 0, 1, 15, 52, "AMbgen" ); regMap["AMbgen" ] = &AMACv2Reg::AMbgen;
  AMbg   .initReg(m_cfg, RW, 0, 5,  8, 52, "AMbg"   ); regMap["AMbg"   ] = &AMACv2Reg::AMbg;
  VDDbgen.initReg(m_cfg, RW, 0, 1,  7, 52, "VDDbgen"); regMap["VDDbgen"] = &AMACv2Reg::VDDbgen;
  VDDbg  .initReg(m_cfg, RW, 0, 5,  0, 52, "VDDbg"  ); regMap["VDDbg"  ] = &AMACv2Reg::VDDbg;
  // 53 - AMcnt
  AMintCalib.initReg(m_cfg, RW, 0, 4, 24, 53, "AMintCalib"); regMap["AMintCalib"] = &AMACv2Reg::AMintCalib;
  Ch13Mux.initReg(m_cfg, RW, 0, 2, 20, 53, "Ch13Mux"); regMap["Ch13Mux"] = &AMACv2Reg::Ch13Mux;
  Ch12Mux.initReg(m_cfg, RW, 0, 2, 16, 53, "Ch12Mux"); regMap["Ch12Mux"] = &AMACv2Reg::Ch12Mux;
  Ch5Mux.initReg(m_cfg, RW, 0, 2, 13, 53, "Ch5Mux"); regMap["Ch5Mux"] = &AMACv2Reg::Ch5Mux;
  Ch4Mux.initReg(m_cfg, RW, 0, 2, 8, 53, "Ch4Mux"); regMap["Ch4Mux"] = &AMACv2Reg::Ch4Mux;
  Ch3Mux.initReg(m_cfg, RW, 0, 2, 4, 53, "Ch3Mux"); regMap["Ch3Mux"] = &AMACv2Reg::Ch3Mux;
  // 54 - Dacs0
  DACShunty.initReg(m_cfg, RW, 0, 8, 24, 54, "DACShunty"); regMap["DACShunty"] = &AMACv2Reg::DACShunty;
  DACShuntx.initReg(m_cfg, RW, 0, 8, 16, 54, "DACShuntx"); regMap["DACShuntx"] = &AMACv2Reg::DACShuntx;
  DACCALy.initReg(m_cfg, RW, 0, 8, 8, 54, "DACCALy"); regMap["DACCALy"] = &AMACv2Reg::DACCALy;
  DACCalx.initReg(m_cfg, RW, 0, 8, 0, 54, "DACCalx"); regMap["DACCalx"] = &AMACv2Reg::DACCalx;
  // 55 - DACbias
  DACbias.initReg(m_cfg, RW, 0xD, 5, 0, 55, "DACbias"); regMap["DACbias"] = &AMACv2Reg::DACbias;
  // 56 - AMACcnt
  HVcurGain .initReg(m_cfg, RW, 0, 4, 16, 56, "HVcurGain" ); regMap["HVcurGain" ] = &AMACv2Reg::HVcurGain;
  DRcomMode .initReg(m_cfg, RW, 0, 2, 12, 56, "DRcomMode" ); regMap["DRcomMode" ] = &AMACv2Reg::DRcomMode;
  DRcurr    .initReg(m_cfg, RW, 4, 3,  8, 56, "DRcurr"    ); regMap["DRcurr"    ] = &AMACv2Reg::DRcurr;
  RingOscFrq.initReg(m_cfg, RW, 4, 3,  0, 56, "RingOscFrq"); regMap["RingOscFrq"] = &AMACv2Reg::RingOscFrq;
  // 57 - NTC
  NTCpbCal.initReg(m_cfg, RW, 0, 1, 19, 57, "NTCpbCal"); regMap["NTCpbCal"] = &AMACv2Reg::NTCpbCal;
  NTCpbSenseRange.initReg(m_cfg, RW, 4, 3, 16, 57, "NTCpbSenseRange"); regMap["NTCpbSenseRange"] = &AMACv2Reg::NTCpbSenseRange;
  NTCy0Cal.initReg(m_cfg, RW, 0, 1, 11, 57, "NTCy0Cal"); regMap["NTCy0Cal"] = &AMACv2Reg::NTCy0Cal;
  NTCy0SenseRange.initReg(m_cfg, RW, 4, 3, 8, 57, "NTCy0SenseRange"); regMap["NTCy0SenseRange"] = &AMACv2Reg::NTCy0SenseRange;
  NTCx0Cal.initReg(m_cfg, RW, 0, 1, 3, 57, "NTCx0Cal"); regMap["NTCx0Cal"] = &AMACv2Reg::NTCx0Cal;
  NTCx0SenseRange.initReg(m_cfg, RW, 4, 3, 0, 57, "NTCx0SenseRange"); regMap["NTCx0SenseRange"] = &AMACv2Reg::NTCx0SenseRange;
  // 58 - LvCurCal
  DCDCoOffset.initReg(m_cfg, RW, 4, 4, 20, 58, "DCDCoOffset"); regMap["DCDCoOffset"] = &AMACv2Reg::DCDCoOffset;
  DCDCoZeroReading.initReg(m_cfg, RW, 0, 1, 19, 58, "DCDCoZeroReading"); regMap["DCDCoZeroReading"] = &AMACv2Reg::DCDCoZeroReading;
  DCDCoP.initReg(m_cfg, RW, 0, 1, 17, 58, "DCDCoP"); regMap["DCDCoP"] = &AMACv2Reg::DCDCoP;
  DCDCoN.initReg(m_cfg, RW, 0, 1, 16, 58, "DCDCoN"); regMap["DCDCoN"] = &AMACv2Reg::DCDCoN;
  DCDCiZeroReading.initReg(m_cfg, RW, 0, 1, 15, 58, "DCDCiZeroReading"); regMap["DCDCiZeroReading"] = &AMACv2Reg::DCDCiZeroReading;
  DCDCiRangeSW.initReg(m_cfg, RW, 1, 1, 12, 58, "DCDCiRangeSW"); regMap["DCDCiRangeSW"] = &AMACv2Reg::DCDCiRangeSW;
  DCDCiOffset.initReg(m_cfg, RW, 8, 4, 8, 58, "DCDCiOffset"); regMap["DCDCiOffset"] = &AMACv2Reg::DCDCiOffset;
  DCDCiP.initReg(m_cfg, RW, 0, 3, 4, 58, "DCDCiP"); regMap["DCDCiP"] = &AMACv2Reg::DCDCiP;
  DCDCiN.initReg(m_cfg, RW, 0, 3, 0, 58, "DCDCiN"); regMap["DCDCiN"] = &AMACv2Reg::DCDCiN;
  // 60 - HxICm_cfg
  HxLAM.initReg(m_cfg, RW, 0, 1, 16, 60, "HxLAM"); regMap["HxLAM"] = &AMACv2Reg::HxLAM;
  HxFlagsLatch.initReg(m_cfg, RW, 1, 1, 12, 60, "HxFlagsLatch"); regMap["HxFlagsLatch"] = &AMACv2Reg::HxFlagsLatch;
  HxFlagsLogic.initReg(m_cfg, RW, 0, 1, 8, 60, "HxFlagsLogic"); regMap["HxFlagsLogic"] = &AMACv2Reg::HxFlagsLogic;
  HxFlagValid.initReg(m_cfg, RW, 0, 1, 4, 60, "HxFlagValid"); regMap["HxFlagValid"] = &AMACv2Reg::HxFlagsLogic;
  HxFlagsValidConf.initReg(m_cfg, RW, 0, 2, 0, 60, "HxFlagValid"); regMap["HxFlagValid"] = &AMACv2Reg::HxFlagsLogic;
  // 61 - HyICm_cfg
  HyLAM.initReg(m_cfg, RW, 0, 1, 16, 61, "HyLAM"); regMap["HyLAM"] = &AMACv2Reg::HyLAM;
  HyFlagsLatch.initReg(m_cfg, RW, 1, 1, 12, 61, "HyFlagsLatch"); regMap["HyFlagsLatch"] = &AMACv2Reg::HyFlagsLatch;
  HyFlagsLogic.initReg(m_cfg, RW, 0, 1, 8, 61, "HyFlagsLogic"); regMap["HyFlagsLogic"] = &AMACv2Reg::HyFlagsLogic;
  HyFlagValid.initReg(m_cfg, RW, 0, 1, 4, 61, "HyFlagValid"); regMap["HyFlagValid"] = &AMACv2Reg::HyFlagsLogic;
  HyFlagsValidConf.initReg(m_cfg, RW, 0, 2, 0, 61, "HyFlagValid"); regMap["HyFlagValid"] = &AMACv2Reg::HyFlagsLogic;
  // 62 - HV0ICm_cfg
  HV0LAM.initReg(m_cfg, RW, 0, 1, 16, 62, "HV0LAM"); regMap["HV0LAM"] = &AMACv2Reg::HV0LAM;
  HV0FlagsLatch.initReg(m_cfg, RW, 1, 1, 12, 62, "HV0FlagsLatch"); regMap["HV0FlagsLatch"] = &AMACv2Reg::HV0FlagsLatch;
  HV0FlagsLogic.initReg(m_cfg, RW, 0, 1, 8, 62, "HV0FlagsLogic"); regMap["HV0FlagsLogic"] = &AMACv2Reg::HV0FlagsLogic;
  HV0FlagValid.initReg(m_cfg, RW, 0, 1, 4, 62, "HV0FlagValid"); regMap["HV0FlagValid"] = &AMACv2Reg::HV0FlagsLogic;
  HV0FlagsValidConf.initReg(m_cfg, RW, 0, 2, 0, 62, "HV0FlagValid"); regMap["HV0FlagValid"] = &AMACv2Reg::HV0FlagsLogic;
  // 63 - HV2ICm_cfg
  HV2LAM.initReg(m_cfg, RW, 0, 1, 16, 63, "HV2LAM"); regMap["HV2LAM"] = &AMACv2Reg::HV2LAM;
  HV2FlagsLatch.initReg(m_cfg, RW, 1, 1, 12, 63, "HV2FlagsLatch"); regMap["HV2FlagsLatch"] = &AMACv2Reg::HV2FlagsLatch;
  HV2FlagsLogic.initReg(m_cfg, RW, 0, 1, 8, 63, "HV2FlagsLogic"); regMap["HV2FlagsLogic"] = &AMACv2Reg::HV2FlagsLogic;
  HV2FlagValid.initReg(m_cfg, RW, 0, 1, 4, 63, "HV2FlagValid"); regMap["HV2FlagValid"] = &AMACv2Reg::HV2FlagsLogic;
  HV2FlagsValidConf.initReg(m_cfg, RW, 0, 2, 0, 63, "HV2FlagValid"); regMap["HV2FlagValid"] = &AMACv2Reg::HV2FlagsLogic;
  // 64 - DCDCICm_cfg
  DCDCLAM.initReg(m_cfg, RW, 0, 1, 16, 64, "DCDCLAM"); regMap["DCDCLAM"] = &AMACv2Reg::DCDCLAM;
  DCDCFlagsLatch.initReg(m_cfg, RW, 1, 1, 12, 64, "DCDCFlagsLatch"); regMap["DCDCFlagsLatch"] = &AMACv2Reg::DCDCFlagsLatch;
  DCDCFlagsLogic.initReg(m_cfg, RW, 0, 1, 8, 64, "DCDCFlagsLogic"); regMap["DCDCFlagsLogic"] = &AMACv2Reg::DCDCFlagsLogic;
  DCDCFlagValid.initReg(m_cfg, RW, 0, 1, 4, 64, "DCDCFlagValid"); regMap["DCDCFlagValid"] = &AMACv2Reg::DCDCFlagsLogic;
  DCDCFlagsValidConf.initReg(m_cfg, RW, 0, 2, 0, 64, "DCDCFlagValid"); regMap["DCDCFlagValid"] = &AMACv2Reg::DCDCFlagsLogic;
  // 65 - WRNICm_cfg
  WRNLAM.initReg(m_cfg, RW, 0, 1, 16, 65, "WRNLAM"); regMap["WRNLAM"] = &AMACv2Reg::WRNLAM;
  WRNFlagsLatch.initReg(m_cfg, RW, 1, 1, 12, 65, "WRNFlagsLatch"); regMap["WRNFlagsLatch"] = &AMACv2Reg::WRNFlagsLatch;
  WRNFlagsLogic.initReg(m_cfg, RW, 0, 1, 8, 65, "WRNFlagsLogic"); regMap["WRNFlagsLogic"] = &AMACv2Reg::WRNFlagsLogic;
  WRNFlagValid.initReg(m_cfg, RW, 0, 1, 4, 65, "WRNFlagValid"); regMap["WRNFlagValid"] = &AMACv2Reg::WRNFlagsLogic;
  WRNFlagsValidConf.initReg(m_cfg, RW, 0, 2, 0, 65, "WRNFlagValid"); regMap["WRNFlagValid"] = &AMACv2Reg::WRNFlagsLogic;
  // 70 - HxTlut
  HxTlut.initReg(m_cfg, RW, 0, 8, 0, 70, "HxTlut"); regMap["HxTlut"] = &AMACv2Reg::HxTlut;
  // 71 - HxModlut1
  HxModlut1.initReg(m_cfg, RW, 0, 32, 0, 71, "HxModlut1"); regMap["HxModlut1"] = &AMACv2Reg::HxModlut1;
  // 72 - HxModlut2
  HxModlut2.initReg(m_cfg, RW, 0, 32, 0, 72, "HxModlut2"); regMap["HxModlut2"] = &AMACv2Reg::HxModlut2;
  // 73 - HyTlut
  HyTlut.initReg(m_cfg, RW, 0, 8, 0, 73, "HyTlut"); regMap["HyTlut"] = &AMACv2Reg::HyTlut;
  // 74 - HyModlut1
  HyModlut1.initReg(m_cfg, RW, 0, 32, 0, 74, "HyModlut1"); regMap["HyModlut1"] = &AMACv2Reg::HyModlut1;
  // 75 - HyModlut2
  HyModlut2.initReg(m_cfg, RW, 0, 32, 0, 75, "HyModlut2"); regMap["HyModlut2"] = &AMACv2Reg::HyModlut2;
  // 76 - HV0Tlut;
  HV0Tlut.initReg(m_cfg, RW, 0, 8, 0, 76, "HV0Tlut"); regMap["HV0Tlut"] = &AMACv2Reg::HV0Tlut;
  // 77 - HV0Modlut1
  HV0Modlut1.initReg(m_cfg, RW, 0, 32, 0, 77, "HV0Modlut1"); regMap["HV0Modlut1"] = &AMACv2Reg::HV0Modlut1;
  // 78 - HV0Modlut2
  HV0Modlut2.initReg(m_cfg, RW, 0, 32, 0, 78, "HV0Modlut2"); regMap["HV0Modlut2"] = &AMACv2Reg::HV0Modlut2;
  // 79 - HV2Tlut
  HV2Tlut.initReg(m_cfg, RW, 0, 8, 0, 79, "HV2Tlut"); regMap["HV2Tlut"] = &AMACv2Reg::HV2Tlut;
  // 80 - HV2Modlut1
  HV2Modlut1.initReg(m_cfg, RW, 0, 32, 0, 80, "HV2Modlut1"); regMap["HV2Modlut1"] = &AMACv2Reg::HV2Modlut1;
  // 81 - HV2Modlut2
  HV2Modlut2.initReg(m_cfg, RW, 0, 32, 0, 81, "HV2Modlut2"); regMap["HV2Modlut2"] = &AMACv2Reg::HV2Modlut2;
  // 82 - DCDCTlut
  DCDCTlut.initReg(m_cfg, RW, 0, 8, 0, 82, "DCDCTlut"); regMap["DCDCTlut"] = &AMACv2Reg::DCDCTlut;
  // 83 - DCDCModlut1
  DCDCModlut1.initReg(m_cfg, RW, 0, 32, 0, 83, "DCDCModlut1"); regMap["DCDCModlut1"] = &AMACv2Reg::DCDCModlut1;
  // 84 - DCDCModlut2
  DCDCModlut2.initReg(m_cfg, RW, 0, 32, 0, 84, "DCDCModlut2"); regMap["DCDCModlut2"] = &AMACv2Reg::DCDCModlut2;
  // 85 - WRNTlut
  WRNTlut.initReg(m_cfg, RW, 0, 8, 0, 85, "WRNTlut"); regMap["WRNTlut"] = &AMACv2Reg::WRNTlut;
  // 86 - WRNModlut1
  WRNModlut1.initReg(m_cfg, RW, 0, 32, 0, 86, "WRNModlut1"); regMap["WRNModlut1"] = &AMACv2Reg::WRNModlut1;
  // 87 - WRNModlut2
  WRNModlut2.initReg(m_cfg, RW, 0, 32, 0, 87, "WRNModlut2"); regMap["WRNModlut2"] = &AMACv2Reg::WRNModlut2;
  // 90 - HxFlagEn
  HxFlagsEnHi.initReg(m_cfg, RW, 0, 16, 16, 90, "HxFlagsEnHi"); regMap["HxFlagsEnHi"] = &AMACv2Reg::HxFlagsEnHi;
  HxFlagsEnLo.initReg(m_cfg, RW, 0, 16, 16, 90, "HxFlagsEnLo"); regMap["HxFlagsEnLo"] = &AMACv2Reg::HxFlagsEnLo;
  // 91 - HyFlagEn
  HyFlagsEnHi.initReg(m_cfg, RW, 0, 16, 16, 91, "HyFlagsEnHi"); regMap["HyFlagsEnHi"] = &AMACv2Reg::HyFlagsEnHi;
  HyFlagsEnLo.initReg(m_cfg, RW, 0, 16, 16, 91, "HyFlagsEnLo"); regMap["HyFlagsEnLo"] = &AMACv2Reg::HyFlagsEnLo;
  // 92 - HV0FlagEn
  HV0FlagsEnHi.initReg(m_cfg, RW, 0, 16, 16, 92, "HV0FlagsEnHi"); regMap["HV0FlagsEnHi"] = &AMACv2Reg::HV0FlagsEnHi;
  HV0FlagsEnLo.initReg(m_cfg, RW, 0, 16, 16, 92, "HV0FlagsEnLo"); regMap["HV0FlagsEnLo"] = &AMACv2Reg::HV0FlagsEnLo;
  // 93 - HV2FlagEn
  HV2FlagsEnHi.initReg(m_cfg, RW, 0, 16, 16, 93, "HxFlagsEnHi"); regMap["HxFlagsEnHi"] = &AMACv2Reg::HxFlagsEnHi;
  HV2FlagsEnLo.initReg(m_cfg, RW, 0, 16, 16, 93, "HxFlagsEnLo"); regMap["HxFlagsEnLo"] = &AMACv2Reg::HxFlagsEnLo;
  // 94 - DCDCFlagEn
  DCDCFlagsEnHi.initReg(m_cfg, RW, 0, 16, 16, 94, "DCDCFlagsEnHi"); regMap["DCDCFlagsEnHi"] = &AMACv2Reg::DCDCFlagsEnHi;
  DCDCFlagsEnLo.initReg(m_cfg, RW, 0, 16, 16, 94, "DCDCFlagsEnLo"); regMap["DCDCFlagsEnLo"] = &AMACv2Reg::DCDCFlagsEnLo;
  // 94 - WRNFlagEn
  WRNFlagsEnHi.initReg(m_cfg, RW, 0, 16, 16, 95, "WRNFlagsEnHi"); regMap["WRNFlagsEnHi"] = &AMACv2Reg::WRNFlagsEnHi;
  WRNFlagsEnLo.initReg(m_cfg, RW, 0, 16, 16, 95, "WRNFlagsEnLo"); regMap["WRNFlagsEnLo"] = &AMACv2Reg::WRNFlagsEnLo;
  // 95 - SynFlagEn
  WRNsynFlagEnHi.initReg(m_cfg, RW, 0, 1, 21, 95, "WRNsynFlagEnHi"); regMap["WRNsynFlagEnHi"] = &AMACv2Reg::WRNsynFlagEnHi;
  WRNsynFlagEnLo.initReg(m_cfg, RW, 0, 1, 20, 95, "WRNsynFlagEnLo"); regMap["WRNsynFlagEnLo"] = &AMACv2Reg::WRNsynFlagEnLo;
  DCDCsynFlagEnHi.initReg(m_cfg, RW, 0, 1, 17, 95, "DCDCsynFlagEnHi"); regMap["DCDCsynFlagEnHi"] = &AMACv2Reg::DCDCsynFlagEnHi;
  DCDCsynFlagEnLo.initReg(m_cfg, RW, 0, 1, 16, 95, "DCDCsynFlagEnLo"); regMap["DCDCsynFlagEnLo"] = &AMACv2Reg::DCDCsynFlagEnLo;
  HV2synFlagEnHi.initReg(m_cfg, RW, 0, 1, 13, 95, "HV2synFlagEnHi"); regMap["HV2synFlagEnHi"] = &AMACv2Reg::HV2synFlagEnHi;
  HV2synFlagEnLo.initReg(m_cfg, RW, 0, 1, 12, 95, "HV2synFlagEnLo"); regMap["HV2synFlagEnLo"] = &AMACv2Reg::HV2synFlagEnLo;
  HV0synFlagEnHi.initReg(m_cfg, RW, 0, 1, 9, 95, "HV0synFlagEnHi"); regMap["HV0synFlagEnHi"] = &AMACv2Reg::HV0synFlagEnHi;
  HV0synFlagEnLo.initReg(m_cfg, RW, 0, 1, 8, 95, "HV0synFlagEnLo"); regMap["HV0synFlagEnLo"] = &AMACv2Reg::HV0synFlagEnLo;
  HysynFlagEnHi.initReg(m_cfg, RW, 0, 1, 5, 95, "HysynFlagEnHi"); regMap["HysynFlagEnHi"] = &AMACv2Reg::HysynFlagEnHi;
  HysynFlagEnLo.initReg(m_cfg, RW, 0, 1, 4, 95, "HysynFlagEnLo"); regMap["HysynFlagEnLo"] = &AMACv2Reg::HysynFlagEnLo;
  HxsynFlagEnHi.initReg(m_cfg, RW, 0, 1, 1, 95, "HxsynFlagEnHi"); regMap["HxsynFlagEnHi"] = &AMACv2Reg::HxsynFlagEnHi;
  HxsynFlagEnLo.initReg(m_cfg, RW, 0, 1, 0, 95, "HxsynFlagEnLo"); regMap["HxsynFlagEnLo"] = &AMACv2Reg::HxsynFlagEnLo;
  // 100 - HxLoTh0
  HxLoThCh0.initReg(m_cfg, RW, 0, 10, 0, 100, "HxLoThCh0"); regMap["HxLoThCh0"] = &AMACv2Reg::HxLoThCh0;
  HxLoThCh1.initReg(m_cfg, RW, 0, 10, 10, 100, "HxLoThCh1"); regMap["HxLoThCh1"] = &AMACv2Reg::HxLoThCh1;
  HxLoThCh2.initReg(m_cfg, RW, 0, 10, 20, 100, "HxLoThCh2"); regMap["HxLoThCh2"] = &AMACv2Reg::HxLoThCh2;
  // 101 - HxLoTh1
  HxLoThCh3.initReg(m_cfg, RW, 0, 10, 0, 101, "HxLoThCh3"); regMap["HxLoThCh3"] = &AMACv2Reg::HxLoThCh3;
  HxLoThCh4.initReg(m_cfg, RW, 0, 10, 10, 101, "HxLoThCh4"); regMap["HxLoThCh4"] = &AMACv2Reg::HxLoThCh4;
  HxLoThCh5.initReg(m_cfg, RW, 0, 10, 20, 101, "HxLoThCh5"); regMap["HxLoThCh5"] = &AMACv2Reg::HxLoThCh5;
  // 102 - HxLoTh2
  HxLoThCh6.initReg(m_cfg, RW, 0, 10, 0, 102, "HxLoThCh6"); regMap["HxLoThCh6"] = &AMACv2Reg::HxLoThCh6;
  HxLoThCh7.initReg(m_cfg, RW, 0, 10, 10, 102, "HxLoThCh7"); regMap["HxLoThCh7"] = &AMACv2Reg::HxLoThCh7;
  HxLoThCh8.initReg(m_cfg, RW, 0, 10, 20, 102, "HxLoThCh8"); regMap["HxLoThCh8"] = &AMACv2Reg::HxLoThCh8;
  // 103 - HxLoTh3
  HxLoThCh9.initReg(m_cfg, RW, 0, 10, 0, 103, "HxLoThCh9"); regMap["HxLoThCh9"] = &AMACv2Reg::HxLoThCh9;
  HxLoThCh10.initReg(m_cfg, RW, 0, 10, 10, 103, "HxLoThCh10"); regMap["HxLoThCh10"] = &AMACv2Reg::HxLoThCh10;
  HxLoThCh11.initReg(m_cfg, RW, 0, 10, 20, 103, "HxLoThCh11"); regMap["HxLoThCh11"] = &AMACv2Reg::HxLoThCh11;
  // 104 - HxLoTh4
  HxLoThCh12.initReg(m_cfg, RW, 0, 10, 0, 104, "HxLoThCh12"); regMap["HxLoThCh12"] = &AMACv2Reg::HxLoThCh12;
  HxLoThCh13.initReg(m_cfg, RW, 0, 10, 10, 104, "HxLoThCh13"); regMap["HxLoThCh13"] = &AMACv2Reg::HxLoThCh13;
  HxLoThCh14.initReg(m_cfg, RW, 0, 10, 20, 104, "HxLoThCh14"); regMap["HxLoThCh14"] = &AMACv2Reg::HxLoThCh14;
  // 105 - HxLoTh5
  HxLoThCh15.initReg(m_cfg, RW, 0, 10, 0, 105, "HxLoThCh15"); regMap["HxLoThCh15"] = &AMACv2Reg::HxLoThCh15;
  // 106 - HxHiTh0
  HxHiThCh0.initReg(m_cfg, RW, 0x3FF, 10, 0, 106, "HxLoThCh0"); regMap["HxLoThCh0"] = &AMACv2Reg::HxLoThCh0;
  HxHiThCh1.initReg(m_cfg, RW, 0x3FF, 10, 10, 106, "HxLoThCh1"); regMap["HxLoThCh1"] = &AMACv2Reg::HxLoThCh1;
  HxHiThCh2.initReg(m_cfg, RW, 0x3FF, 10, 20, 106, "HxLoThCh2"); regMap["HxLoThCh2"] = &AMACv2Reg::HxLoThCh2;
  // 107 - HxHiTh1
  HxHiThCh3.initReg(m_cfg, RW, 0x3FF, 10, 0, 107, "HxHiThCh3"); regMap["HxHiThCh3"] = &AMACv2Reg::HxHiThCh3;
  HxHiThCh4.initReg(m_cfg, RW, 0x3FF, 10, 10, 107, "HxHiThCh4"); regMap["HxHiThCh4"] = &AMACv2Reg::HxHiThCh4;
  HxHiThCh5.initReg(m_cfg, RW, 0x3FF, 10, 20, 107, "HxHiThCh5"); regMap["HxHiThCh5"] = &AMACv2Reg::HxHiThCh5;
  // 108 - HxHiTh2
  HxHiThCh6.initReg(m_cfg, RW, 0x3FF, 10, 0, 108, "HxHiThCh6"); regMap["HxHiThCh6"] = &AMACv2Reg::HxHiThCh6;
  HxHiThCh7.initReg(m_cfg, RW, 0x3FF, 10, 10, 108, "HxHiThCh7"); regMap["HxHiThCh7"] = &AMACv2Reg::HxHiThCh7;
  HxHiThCh8.initReg(m_cfg, RW, 0x3FF, 10, 20, 108, "HxHiThCh8"); regMap["HxHiThCh8"] = &AMACv2Reg::HxHiThCh8;
  // 109 - HxHiTh3
  HxHiThCh9.initReg(m_cfg, RW, 0x3FF, 10, 0, 109, "HxHiThCh9"); regMap["HxHiThCh9"] = &AMACv2Reg::HxHiThCh9;
  HxHiThCh10.initReg(m_cfg, RW, 0x3FF, 10, 10, 109, "HxHiThCh10"); regMap["HxHiThCh10"] = &AMACv2Reg::HxHiThCh10;
  HxHiThCh11.initReg(m_cfg, RW, 0x3FF, 10, 20, 109, "HxHiThCh11"); regMap["HxHiThCh11"] = &AMACv2Reg::HxHiThCh11;
  // 110 - HxHiTh4
  HxHiThCh12.initReg(m_cfg, RW, 0x3FF, 10, 0, 110, "HxHiThCh12"); regMap["HxHiThCh12"] = &AMACv2Reg::HxHiThCh12;
  HxHiThCh13.initReg(m_cfg, RW, 0x3FF, 10, 10, 110, "HxHiThCh13"); regMap["HxHiThCh13"] = &AMACv2Reg::HxHiThCh13;
  HxHiThCh14.initReg(m_cfg, RW, 0x3FF, 10, 20, 110, "HxHiThCh14"); regMap["HxHiThCh14"] = &AMACv2Reg::HxHiThCh14;
  // 111 - HxHiTh5
  HxHiThCh15.initReg(m_cfg, RW, 0x3FF, 10, 0, 111, "HxHiThCh15"); regMap["HxHiThCh15"] = &AMACv2Reg::HxHiThCh15;
  // 112 - HyLoTh0
  HyLoThCh0.initReg(m_cfg, RW, 0, 10, 0, 112, "HyLoThCh0"); regMap["HyLoThCh0"] = &AMACv2Reg::HyLoThCh0;
  HyLoThCh1.initReg(m_cfg, RW, 0, 10, 10, 112, "HyLoThCh1"); regMap["HyLoThCh1"] = &AMACv2Reg::HyLoThCh1;
  HyLoThCh2.initReg(m_cfg, RW, 0, 10, 20, 112, "HyLoThCh2"); regMap["HyLoThCh2"] = &AMACv2Reg::HyLoThCh2;
  // 113 - HyLoTh1
  HyLoThCh3.initReg(m_cfg, RW, 0, 10, 0, 113, "HyLoThCh3"); regMap["HyLoThCh3"] = &AMACv2Reg::HyLoThCh3;
  HyLoThCh4.initReg(m_cfg, RW, 0, 10, 10, 113, "HyLoThCh4"); regMap["HyLoThCh4"] = &AMACv2Reg::HyLoThCh4;
  HyLoThCh5.initReg(m_cfg, RW, 0, 10, 20, 113, "HyLoThCh5"); regMap["HyLoThCh5"] = &AMACv2Reg::HyLoThCh5;
  // 114 - HyLoTh2
  HyLoThCh6.initReg(m_cfg, RW, 0, 10, 0, 114, "HyLoThCh6"); regMap["HyLoThCh6"] = &AMACv2Reg::HyLoThCh6;
  HyLoThCh7.initReg(m_cfg, RW, 0, 10, 10, 114, "HyLoThCh7"); regMap["HyLoThCh7"] = &AMACv2Reg::HyLoThCh7;
  HyLoThCh8.initReg(m_cfg, RW, 0, 10, 20, 114, "HyLoThCh8"); regMap["HyLoThCh8"] = &AMACv2Reg::HyLoThCh8;
  // 115 - HyLoTh3
  HyLoThCh9.initReg(m_cfg, RW, 0, 10, 0, 115, "HyLoThCh9"); regMap["HyLoThCh9"] = &AMACv2Reg::HyLoThCh9;
  HyLoThCh10.initReg(m_cfg, RW, 0, 10, 10, 115, "HyLoThCh10"); regMap["HyLoThCh10"] = &AMACv2Reg::HyLoThCh10;
  HyLoThCh11.initReg(m_cfg, RW, 0, 10, 20, 115, "HyLoThCh11"); regMap["HyLoThCh11"] = &AMACv2Reg::HyLoThCh11;
  // 116 - HyLoTh4
  HyLoThCh12.initReg(m_cfg, RW, 0, 10, 0, 116, "HyLoThCh12"); regMap["HyLoThCh12"] = &AMACv2Reg::HyLoThCh12;
  HyLoThCh13.initReg(m_cfg, RW, 0, 10, 10, 116, "HyLoThCh13"); regMap["HyLoThCh13"] = &AMACv2Reg::HyLoThCh13;
  HyLoThCh14.initReg(m_cfg, RW, 0, 10, 20, 116, "HyLoThCh14"); regMap["HyLoThCh14"] = &AMACv2Reg::HyLoThCh14;
  // 117 - HyLoTh5
  HyLoThCh15.initReg(m_cfg, RW, 0, 10, 0, 117, "HyLoThCh15"); regMap["HyLoThCh15"] = &AMACv2Reg::HyLoThCh15;
  // 118 - HyHiTh0
  HyHiThCh0.initReg(m_cfg, RW, 0x3FF, 10, 0, 118, "HyLoThCh0"); regMap["HyLoThCh0"] = &AMACv2Reg::HyLoThCh0;
  HyHiThCh1.initReg(m_cfg, RW, 0x3FF, 10, 10, 118, "HyLoThCh1"); regMap["HyLoThCh1"] = &AMACv2Reg::HyLoThCh1;
  HyHiThCh2.initReg(m_cfg, RW, 0x3FF, 10, 20, 118, "HyLoThCh2"); regMap["HyLoThCh2"] = &AMACv2Reg::HyLoThCh2;
  // 119 - HyHiTh1
  HyHiThCh3.initReg(m_cfg, RW, 0x3FF, 10, 0, 119, "HyHiThCh3"); regMap["HyHiThCh3"] = &AMACv2Reg::HyHiThCh3;
  HyHiThCh4.initReg(m_cfg, RW, 0x3FF, 10, 10, 119, "HyHiThCh4"); regMap["HyHiThCh4"] = &AMACv2Reg::HyHiThCh4;
  HyHiThCh5.initReg(m_cfg, RW, 0x3FF, 10, 20, 119, "HyHiThCh5"); regMap["HyHiThCh5"] = &AMACv2Reg::HyHiThCh5;
  // 120 - HyHiTh2
  HyHiThCh6.initReg(m_cfg, RW, 0x3FF, 10, 0, 120, "HyHiThCh6"); regMap["HyHiThCh6"] = &AMACv2Reg::HyHiThCh6;
  HyHiThCh7.initReg(m_cfg, RW, 0x3FF, 10, 10, 120, "HyHiThCh7"); regMap["HyHiThCh7"] = &AMACv2Reg::HyHiThCh7;
  HyHiThCh8.initReg(m_cfg, RW, 0x3FF, 10, 20, 120, "HyHiThCh8"); regMap["HyHiThCh8"] = &AMACv2Reg::HyHiThCh8;
  // 121 - HyHiTh3
  HyHiThCh9.initReg(m_cfg, RW, 0x3FF, 10, 0, 121, "HyHiThCh9"); regMap["HyHiThCh9"] = &AMACv2Reg::HyHiThCh9;
  HyHiThCh10.initReg(m_cfg, RW, 0x3FF, 10, 10, 121, "HyHiThCh10"); regMap["HyHiThCh10"] = &AMACv2Reg::HyHiThCh10;
  HyHiThCh11.initReg(m_cfg, RW, 0x3FF, 10, 20, 121, "HyHiThCh11"); regMap["HyHiThCh11"] = &AMACv2Reg::HyHiThCh11;
  // 122 - HyHiTh4
  HyHiThCh12.initReg(m_cfg, RW, 0x3FF, 10, 0, 122, "HyHiThCh12"); regMap["HyHiThCh12"] = &AMACv2Reg::HyHiThCh12;
  HyHiThCh13.initReg(m_cfg, RW, 0x3FF, 10, 10, 122, "HyHiThCh13"); regMap["HyHiThCh13"] = &AMACv2Reg::HyHiThCh13;
  HyHiThCh14.initReg(m_cfg, RW, 0x3FF, 10, 20, 122, "HyHiThCh14"); regMap["HyHiThCh14"] = &AMACv2Reg::HyHiThCh14;
  // 123 - HyHiTh5
  HyHiThCh15.initReg(m_cfg, RW, 0x3FF, 10, 0, 123, "HyHiThCh15"); regMap["HyHiThCh15"] = &AMACv2Reg::HyHiThCh15;
  // 124 - HV0LoTh0
  HV0LoThCh0.initReg(m_cfg, RW, 0, 10, 0, 124, "HV0LoThCh0"); regMap["HV0LoThCh0"] = &AMACv2Reg::HV0LoThCh0;
  HV0LoThCh1.initReg(m_cfg, RW, 0, 10, 10, 124, "HV0LoThCh1"); regMap["HV0LoThCh1"] = &AMACv2Reg::HV0LoThCh1;
  HV0LoThCh2.initReg(m_cfg, RW, 0, 10, 20, 124, "HV0LoThCh2"); regMap["HV0LoThCh2"] = &AMACv2Reg::HV0LoThCh2;
  // 125 - HV0LoTh1
  HV0LoThCh3.initReg(m_cfg, RW, 0, 10, 0, 125, "HV0LoThCh3"); regMap["HV0LoThCh3"] = &AMACv2Reg::HV0LoThCh3;
  HV0LoThCh4.initReg(m_cfg, RW, 0, 10, 10, 125, "HV0LoThCh4"); regMap["HV0LoThCh4"] = &AMACv2Reg::HV0LoThCh4;
  HV0LoThCh5.initReg(m_cfg, RW, 0, 10, 20, 125, "HV0LoThCh5"); regMap["HV0LoThCh5"] = &AMACv2Reg::HV0LoThCh5;
  // 126 - HV0LoTh2
  HV0LoThCh6.initReg(m_cfg, RW, 0, 10, 0, 126, "HV0LoThCh6"); regMap["HV0LoThCh6"] = &AMACv2Reg::HV0LoThCh6;
  HV0LoThCh7.initReg(m_cfg, RW, 0, 10, 10, 126, "HV0LoThCh7"); regMap["HV0LoThCh7"] = &AMACv2Reg::HV0LoThCh7;
  HV0LoThCh8.initReg(m_cfg, RW, 0, 10, 20, 126, "HV0LoThCh8"); regMap["HV0LoThCh8"] = &AMACv2Reg::HV0LoThCh8;
  // 127 - HV0LoTh3
  HV0LoThCh9.initReg(m_cfg, RW, 0, 10, 0, 127, "HV0LoThCh9"); regMap["HV0LoThCh9"] = &AMACv2Reg::HV0LoThCh9;
  HV0LoThCh10.initReg(m_cfg, RW, 0, 10, 10, 127, "HV0LoThCh10"); regMap["HV0LoThCh10"] = &AMACv2Reg::HV0LoThCh10;
  HV0LoThCh11.initReg(m_cfg, RW, 0, 10, 20, 127, "HV0LoThCh11"); regMap["HV0LoThCh11"] = &AMACv2Reg::HV0LoThCh11;
  // 128 - HV0LoTh4
  HV0LoThCh12.initReg(m_cfg, RW, 0, 10, 0, 128, "HV0LoThCh12"); regMap["HV0LoThCh12"] = &AMACv2Reg::HV0LoThCh12;
  HV0LoThCh13.initReg(m_cfg, RW, 0, 10, 10, 128, "HV0LoThCh13"); regMap["HV0LoThCh13"] = &AMACv2Reg::HV0LoThCh13;
  HV0LoThCh14.initReg(m_cfg, RW, 0, 10, 20, 128, "HV0LoThCh14"); regMap["HV0LoThCh14"] = &AMACv2Reg::HV0LoThCh14;
  // 129 - HV0LoTh5
  HV0LoThCh15.initReg(m_cfg, RW, 0, 10, 0, 129, "HV0LoThCh15"); regMap["HV0LoThCh15"] = &AMACv2Reg::HV0LoThCh15;
  // 130 - HV0HiTh0
  HV0HiThCh0.initReg(m_cfg, RW, 0x3FF, 10, 0, 130, "HV0LoThCh0"); regMap["HV0LoThCh0"] = &AMACv2Reg::HV0LoThCh0;
  HV0HiThCh1.initReg(m_cfg, RW, 0x3FF, 10, 10, 130, "HV0LoThCh1"); regMap["HV0LoThCh1"] = &AMACv2Reg::HV0LoThCh1;
  HV0HiThCh2.initReg(m_cfg, RW, 0x3FF, 10, 20, 130, "HV0LoThCh2"); regMap["HV0LoThCh2"] = &AMACv2Reg::HV0LoThCh2;
  // 131 - HV0HiTh1
  HV0HiThCh3.initReg(m_cfg, RW, 0x3FF, 10, 0, 131, "HV0HiThCh3"); regMap["HV0HiThCh3"] = &AMACv2Reg::HV0HiThCh3;
  HV0HiThCh4.initReg(m_cfg, RW, 0x3FF, 10, 10, 131, "HV0HiThCh4"); regMap["HV0HiThCh4"] = &AMACv2Reg::HV0HiThCh4;
  HV0HiThCh5.initReg(m_cfg, RW, 0x3FF, 10, 20, 131, "HV0HiThCh5"); regMap["HV0HiThCh5"] = &AMACv2Reg::HV0HiThCh5;
  // 132 - HV0HiTh2
  HV0HiThCh6.initReg(m_cfg, RW, 0x3FF, 10, 0, 132, "HV0HiThCh6"); regMap["HV0HiThCh6"] = &AMACv2Reg::HV0HiThCh6;
  HV0HiThCh7.initReg(m_cfg, RW, 0x3FF, 10, 10, 132, "HV0HiThCh7"); regMap["HV0HiThCh7"] = &AMACv2Reg::HV0HiThCh7;
  HV0HiThCh8.initReg(m_cfg, RW, 0x3FF, 10, 20, 132, "HV0HiThCh8"); regMap["HV0HiThCh8"] = &AMACv2Reg::HV0HiThCh8;
  // 133 - HV0HiTh3
  HV0HiThCh9.initReg(m_cfg, RW, 0x3FF, 10, 0, 133, "HV0HiThCh9"); regMap["HV0HiThCh9"] = &AMACv2Reg::HV0HiThCh9;
  HV0HiThCh10.initReg(m_cfg, RW, 0x3FF, 10, 10, 133, "HV0HiThCh10"); regMap["HV0HiThCh10"] = &AMACv2Reg::HV0HiThCh10;
  HV0HiThCh11.initReg(m_cfg, RW, 0x3FF, 10, 20, 133, "HV0HiThCh11"); regMap["HV0HiThCh11"] = &AMACv2Reg::HV0HiThCh11;
  // 134 - HV0HiTh4
  HV0HiThCh12.initReg(m_cfg, RW, 0x3FF, 10, 0, 134, "HV0HiThCh12"); regMap["HV0HiThCh12"] = &AMACv2Reg::HV0HiThCh12;
  HV0HiThCh13.initReg(m_cfg, RW, 0x3FF, 10, 10, 134, "HV0HiThCh13"); regMap["HV0HiThCh13"] = &AMACv2Reg::HV0HiThCh13;
  HV0HiThCh14.initReg(m_cfg, RW, 0x3FF, 10, 20, 134, "HV0HiThCh14"); regMap["HV0HiThCh14"] = &AMACv2Reg::HV0HiThCh14;
  // 135 - HV0HiTh5
  HV0HiThCh15.initReg(m_cfg, RW, 0x3FF, 10, 0, 135, "HV0HiThCh15"); regMap["HV0HiThCh15"] = &AMACv2Reg::HV0HiThCh15;
  // 136 - HV2LoTh0
  HV2LoThCh0.initReg(m_cfg, RW, 0, 10, 0, 136, "HV2LoThCh0"); regMap["HV2LoThCh0"] = &AMACv2Reg::HV2LoThCh0;
  HV2LoThCh1.initReg(m_cfg, RW, 0, 10, 10, 136, "HV2LoThCh1"); regMap["HV2LoThCh1"] = &AMACv2Reg::HV2LoThCh1;
  HV2LoThCh2.initReg(m_cfg, RW, 0, 10, 20, 136, "HV2LoThCh2"); regMap["HV2LoThCh2"] = &AMACv2Reg::HV2LoThCh2;
  // 137 - HV2LoTh1
  HV2LoThCh3.initReg(m_cfg, RW, 0, 10, 0, 137, "HV2LoThCh3"); regMap["HV2LoThCh3"] = &AMACv2Reg::HV2LoThCh3;
  HV2LoThCh4.initReg(m_cfg, RW, 0, 10, 10, 137, "HV2LoThCh4"); regMap["HV2LoThCh4"] = &AMACv2Reg::HV2LoThCh4;
  HV2LoThCh5.initReg(m_cfg, RW, 0, 10, 20, 137, "HV2LoThCh5"); regMap["HV2LoThCh5"] = &AMACv2Reg::HV2LoThCh5;
  // 138 - HV2LoTh2
  HV2LoThCh6.initReg(m_cfg, RW, 0, 10, 0, 138, "HV2LoThCh6"); regMap["HV2LoThCh6"] = &AMACv2Reg::HV2LoThCh6;
  HV2LoThCh7.initReg(m_cfg, RW, 0, 10, 10, 138, "HV2LoThCh7"); regMap["HV2LoThCh7"] = &AMACv2Reg::HV2LoThCh7;
  HV2LoThCh8.initReg(m_cfg, RW, 0, 10, 20, 138, "HV2LoThCh8"); regMap["HV2LoThCh8"] = &AMACv2Reg::HV2LoThCh8;
  // 139 - HV2LoTh3
  HV2LoThCh9.initReg(m_cfg, RW, 0, 10, 0, 139, "HV2LoThCh9"); regMap["HV2LoThCh9"] = &AMACv2Reg::HV2LoThCh9;
  HV2LoThCh10.initReg(m_cfg, RW, 0, 10, 10, 139, "HV2LoThCh10"); regMap["HV2LoThCh10"] = &AMACv2Reg::HV2LoThCh10;
  HV2LoThCh11.initReg(m_cfg, RW, 0, 10, 20, 139, "HV2LoThCh11"); regMap["HV2LoThCh11"] = &AMACv2Reg::HV2LoThCh11;
  // 140 - HV2LoTh4
  HV2LoThCh12.initReg(m_cfg, RW, 0, 10, 0, 140, "HV2LoThCh12"); regMap["HV2LoThCh12"] = &AMACv2Reg::HV2LoThCh12;
  HV2LoThCh13.initReg(m_cfg, RW, 0, 10, 10, 140, "HV2LoThCh13"); regMap["HV2LoThCh13"] = &AMACv2Reg::HV2LoThCh13;
  HV2LoThCh14.initReg(m_cfg, RW, 0, 10, 20, 140, "HV2LoThCh14"); regMap["HV2LoThCh14"] = &AMACv2Reg::HV2LoThCh14;
  // 141 - HV2LoTh5
  HV2LoThCh15.initReg(m_cfg, RW, 0, 10, 0, 141, "HV2LoThCh15"); regMap["HV2LoThCh15"] = &AMACv2Reg::HV2LoThCh15;
  // 142 - HV2HiTh0
  HV2HiThCh0.initReg(m_cfg, RW, 0x3FF, 10, 0, 142, "HV2LoThCh0"); regMap["HV2LoThCh0"] = &AMACv2Reg::HV2LoThCh0;
  HV2HiThCh1.initReg(m_cfg, RW, 0x3FF, 10, 10, 142, "HV2LoThCh1"); regMap["HV2LoThCh1"] = &AMACv2Reg::HV2LoThCh1;
  HV2HiThCh2.initReg(m_cfg, RW, 0x3FF, 10, 20, 142, "HV2LoThCh2"); regMap["HV2LoThCh2"] = &AMACv2Reg::HV2LoThCh2;
  // 143 - HV2HiTh1
  HV2HiThCh3.initReg(m_cfg, RW, 0x3FF, 10, 0, 143, "HV2HiThCh3"); regMap["HV2HiThCh3"] = &AMACv2Reg::HV2HiThCh3;
  HV2HiThCh4.initReg(m_cfg, RW, 0x3FF, 10, 10, 143, "HV2HiThCh4"); regMap["HV2HiThCh4"] = &AMACv2Reg::HV2HiThCh4;
  HV2HiThCh5.initReg(m_cfg, RW, 0x3FF, 10, 20, 143, "HV2HiThCh5"); regMap["HV2HiThCh5"] = &AMACv2Reg::HV2HiThCh5;
  // 144 - HV2HiTh2
  HV2HiThCh6.initReg(m_cfg, RW, 0x3FF, 10, 0, 144, "HV2HiThCh6"); regMap["HV2HiThCh6"] = &AMACv2Reg::HV2HiThCh6;
  HV2HiThCh7.initReg(m_cfg, RW, 0x3FF, 10, 10, 144, "HV2HiThCh7"); regMap["HV2HiThCh7"] = &AMACv2Reg::HV2HiThCh7;
  HV2HiThCh8.initReg(m_cfg, RW, 0x3FF, 10, 20, 144, "HV2HiThCh8"); regMap["HV2HiThCh8"] = &AMACv2Reg::HV2HiThCh8;
  // 145 - HV2HiTh3
  HV2HiThCh9.initReg(m_cfg, RW, 0x3FF, 10, 0, 145, "HV2HiThCh9"); regMap["HV2HiThCh9"] = &AMACv2Reg::HV2HiThCh9;
  HV2HiThCh10.initReg(m_cfg, RW, 0x3FF, 10, 10, 145, "HV2HiThCh10"); regMap["HV2HiThCh10"] = &AMACv2Reg::HV2HiThCh10;
  HV2HiThCh11.initReg(m_cfg, RW, 0x3FF, 10, 20, 145, "HV2HiThCh11"); regMap["HV2HiThCh11"] = &AMACv2Reg::HV2HiThCh11;
  // 146 - HV2HiTh4
  HV2HiThCh12.initReg(m_cfg, RW, 0x3FF, 10, 0, 146, "HV2HiThCh12"); regMap["HV2HiThCh12"] = &AMACv2Reg::HV2HiThCh12;
  HV2HiThCh13.initReg(m_cfg, RW, 0x3FF, 10, 10, 146, "HV2HiThCh13"); regMap["HV2HiThCh13"] = &AMACv2Reg::HV2HiThCh13;
  HV2HiThCh14.initReg(m_cfg, RW, 0x3FF, 10, 20, 146, "HV2HiThCh14"); regMap["HV2HiThCh14"] = &AMACv2Reg::HV2HiThCh14;
  // 147 - HV2HiTh5
  HV2HiThCh15.initReg(m_cfg, RW, 0x3FF, 10, 0, 147, "HV2HiThCh15"); regMap["HV2HiThCh15"] = &AMACv2Reg::HV2HiThCh15;
  // 148 - DCDCLoTh0
  DCDCLoThCh0.initReg(m_cfg, RW, 0, 10, 0, 148, "DCDCLoThCh0"); regMap["DCDCLoThCh0"] = &AMACv2Reg::DCDCLoThCh0;
  DCDCLoThCh1.initReg(m_cfg, RW, 0, 10, 10, 148, "DCDCLoThCh1"); regMap["DCDCLoThCh1"] = &AMACv2Reg::DCDCLoThCh1;
  DCDCLoThCh2.initReg(m_cfg, RW, 0, 10, 20, 148, "DCDCLoThCh2"); regMap["DCDCLoThCh2"] = &AMACv2Reg::DCDCLoThCh2;
  // 149 - DCDCLoTh1
  DCDCLoThCh3.initReg(m_cfg, RW, 0, 10, 0, 149, "DCDCLoThCh3"); regMap["DCDCLoThCh3"] = &AMACv2Reg::DCDCLoThCh3;
  DCDCLoThCh4.initReg(m_cfg, RW, 0, 10, 10, 149, "DCDCLoThCh4"); regMap["DCDCLoThCh4"] = &AMACv2Reg::DCDCLoThCh4;
  DCDCLoThCh5.initReg(m_cfg, RW, 0, 10, 20, 149, "DCDCLoThCh5"); regMap["DCDCLoThCh5"] = &AMACv2Reg::DCDCLoThCh5;
  // 150 - DCDCLoTh2
  DCDCLoThCh6.initReg(m_cfg, RW, 0, 10, 0, 150, "DCDCLoThCh6"); regMap["DCDCLoThCh6"] = &AMACv2Reg::DCDCLoThCh6;
  DCDCLoThCh7.initReg(m_cfg, RW, 0, 10, 10, 150, "DCDCLoThCh7"); regMap["DCDCLoThCh7"] = &AMACv2Reg::DCDCLoThCh7;
  DCDCLoThCh8.initReg(m_cfg, RW, 0, 10, 20, 150, "DCDCLoThCh8"); regMap["DCDCLoThCh8"] = &AMACv2Reg::DCDCLoThCh8;
  // 151 - DCDCLoTh3
  DCDCLoThCh9.initReg(m_cfg, RW, 0, 10, 0, 151, "DCDCLoThCh9"); regMap["DCDCLoThCh9"] = &AMACv2Reg::DCDCLoThCh9;
  DCDCLoThCh10.initReg(m_cfg, RW, 0, 10, 10, 151, "DCDCLoThCh10"); regMap["DCDCLoThCh10"] = &AMACv2Reg::DCDCLoThCh10;
  DCDCLoThCh11.initReg(m_cfg, RW, 0, 10, 20, 151, "DCDCLoThCh11"); regMap["DCDCLoThCh11"] = &AMACv2Reg::DCDCLoThCh11;
  // 152 - DCDCLoTh4
  DCDCLoThCh12.initReg(m_cfg, RW, 0, 10, 0, 152, "DCDCLoThCh12"); regMap["DCDCLoThCh12"] = &AMACv2Reg::DCDCLoThCh12;
  DCDCLoThCh13.initReg(m_cfg, RW, 0, 10, 10, 152, "DCDCLoThCh13"); regMap["DCDCLoThCh13"] = &AMACv2Reg::DCDCLoThCh13;
  DCDCLoThCh14.initReg(m_cfg, RW, 0, 10, 20, 152, "DCDCLoThCh14"); regMap["DCDCLoThCh14"] = &AMACv2Reg::DCDCLoThCh14;
  // 153 - DCDCLoTh5
  DCDCLoThCh15.initReg(m_cfg, RW, 0, 10, 0, 153, "DCDCLoThCh15"); regMap["DCDCLoThCh15"] = &AMACv2Reg::DCDCLoThCh15;
  // 154 - DCDCHiTh0
  DCDCHiThCh0.initReg(m_cfg, RW, 0x3FF, 10, 0, 154, "DCDCLoThCh0"); regMap["DCDCLoThCh0"] = &AMACv2Reg::DCDCLoThCh0;
  DCDCHiThCh1.initReg(m_cfg, RW, 0x3FF, 10, 10, 154, "DCDCLoThCh1"); regMap["DCDCLoThCh1"] = &AMACv2Reg::DCDCLoThCh1;
  DCDCHiThCh2.initReg(m_cfg, RW, 0x3FF, 10, 20, 154, "DCDCLoThCh2"); regMap["DCDCLoThCh2"] = &AMACv2Reg::DCDCLoThCh2;
  // 155 - DCDCHiTh1
  DCDCHiThCh3.initReg(m_cfg, RW, 0x3FF, 10, 0, 155, "DCDCHiThCh3"); regMap["DCDCHiThCh3"] = &AMACv2Reg::DCDCHiThCh3;
  DCDCHiThCh4.initReg(m_cfg, RW, 0x3FF, 10, 10, 155, "DCDCHiThCh4"); regMap["DCDCHiThCh4"] = &AMACv2Reg::DCDCHiThCh4;
  DCDCHiThCh5.initReg(m_cfg, RW, 0x3FF, 10, 20, 155, "DCDCHiThCh5"); regMap["DCDCHiThCh5"] = &AMACv2Reg::DCDCHiThCh5;
  // 156 - DCDCHiTh2
  DCDCHiThCh6.initReg(m_cfg, RW, 0x3FF, 10, 0, 156, "DCDCHiThCh6"); regMap["DCDCHiThCh6"] = &AMACv2Reg::DCDCHiThCh6;
  DCDCHiThCh7.initReg(m_cfg, RW, 0x3FF, 10, 10, 156, "DCDCHiThCh7"); regMap["DCDCHiThCh7"] = &AMACv2Reg::DCDCHiThCh7;
  DCDCHiThCh8.initReg(m_cfg, RW, 0x3FF, 10, 20, 156, "DCDCHiThCh8"); regMap["DCDCHiThCh8"] = &AMACv2Reg::DCDCHiThCh8;
  // 157 - DCDCHiTh3
  DCDCHiThCh9.initReg(m_cfg, RW, 0x3FF, 10, 0, 157, "DCDCHiThCh9"); regMap["DCDCHiThCh9"] = &AMACv2Reg::DCDCHiThCh9;
  DCDCHiThCh10.initReg(m_cfg, RW, 0x3FF, 10, 10, 157, "DCDCHiThCh10"); regMap["DCDCHiThCh10"] = &AMACv2Reg::DCDCHiThCh10;
  DCDCHiThCh11.initReg(m_cfg, RW, 0x3FF, 10, 20, 157, "DCDCHiThCh11"); regMap["DCDCHiThCh11"] = &AMACv2Reg::DCDCHiThCh11;
  // 158 - DCDCHiTh4
  DCDCHiThCh12.initReg(m_cfg, RW, 0x3FF, 10, 0, 158, "DCDCHiThCh12"); regMap["DCDCHiThCh12"] = &AMACv2Reg::DCDCHiThCh12;
  DCDCHiThCh13.initReg(m_cfg, RW, 0x3FF, 10, 10, 158, "DCDCHiThCh13"); regMap["DCDCHiThCh13"] = &AMACv2Reg::DCDCHiThCh13;
  DCDCHiThCh14.initReg(m_cfg, RW, 0x3FF, 10, 20, 158, "DCDCHiThCh14"); regMap["DCDCHiThCh14"] = &AMACv2Reg::DCDCHiThCh14;
  // 159 - DCDCHiTh5
  DCDCHiThCh15.initReg(m_cfg, RW, 0x3FF, 10, 0, 159, "DCDCHiThCh15"); regMap["DCDCHiThCh15"] = &AMACv2Reg::DCDCHiThCh15;
  // 160 - WRNLoTh0
  WRNLoThCh0.initReg(m_cfg, RW, 0, 10, 0, 160, "WRNLoThCh0"); regMap["WRNLoThCh0"] = &AMACv2Reg::WRNLoThCh0;
  WRNLoThCh1.initReg(m_cfg, RW, 0, 10, 10, 160, "WRNLoThCh1"); regMap["WRNLoThCh1"] = &AMACv2Reg::WRNLoThCh1;
  WRNLoThCh2.initReg(m_cfg, RW, 0, 10, 20, 160, "WRNLoThCh2"); regMap["WRNLoThCh2"] = &AMACv2Reg::WRNLoThCh2;
  // 161 - WRNLoTh1
  WRNLoThCh3.initReg(m_cfg, RW, 0, 10, 0, 161, "WRNLoThCh3"); regMap["WRNLoThCh3"] = &AMACv2Reg::WRNLoThCh3;
  WRNLoThCh4.initReg(m_cfg, RW, 0, 10, 10, 161, "WRNLoThCh4"); regMap["WRNLoThCh4"] = &AMACv2Reg::WRNLoThCh4;
  WRNLoThCh5.initReg(m_cfg, RW, 0, 10, 20, 161, "WRNLoThCh5"); regMap["WRNLoThCh5"] = &AMACv2Reg::WRNLoThCh5;
  // 162 - WRNLoTh2
  WRNLoThCh6.initReg(m_cfg, RW, 0, 10, 0, 162, "WRNLoThCh6"); regMap["WRNLoThCh6"] = &AMACv2Reg::WRNLoThCh6;
  WRNLoThCh7.initReg(m_cfg, RW, 0, 10, 10, 162, "WRNLoThCh7"); regMap["WRNLoThCh7"] = &AMACv2Reg::WRNLoThCh7;
  WRNLoThCh8.initReg(m_cfg, RW, 0, 10, 20, 162, "WRNLoThCh8"); regMap["WRNLoThCh8"] = &AMACv2Reg::WRNLoThCh8;
  // 163 - WRNLoTh3
  WRNLoThCh9.initReg(m_cfg, RW, 0, 10, 0, 163, "WRNLoThCh9"); regMap["WRNLoThCh9"] = &AMACv2Reg::WRNLoThCh9;
  WRNLoThCh10.initReg(m_cfg, RW, 0, 10, 10, 163, "WRNLoThCh10"); regMap["WRNLoThCh10"] = &AMACv2Reg::WRNLoThCh10;
  WRNLoThCh11.initReg(m_cfg, RW, 0, 10, 20, 163, "WRNLoThCh11"); regMap["WRNLoThCh11"] = &AMACv2Reg::WRNLoThCh11;
  // 164 - WRNLoTh4
  WRNLoThCh12.initReg(m_cfg, RW, 0, 10, 0, 164, "WRNLoThCh12"); regMap["WRNLoThCh12"] = &AMACv2Reg::WRNLoThCh12;
  WRNLoThCh13.initReg(m_cfg, RW, 0, 10, 10, 164, "WRNLoThCh13"); regMap["WRNLoThCh13"] = &AMACv2Reg::WRNLoThCh13;
  WRNLoThCh14.initReg(m_cfg, RW, 0, 10, 20, 164, "WRNLoThCh14"); regMap["WRNLoThCh14"] = &AMACv2Reg::WRNLoThCh14;
  // 165 - WRNLoTh5
  WRNLoThCh15.initReg(m_cfg, RW, 0, 10, 0, 165, "WRNLoThCh15"); regMap["WRNLoThCh15"] = &AMACv2Reg::WRNLoThCh15;
  // 166 - WRNHiTh0
  WRNHiThCh0.initReg(m_cfg, RW, 0x3FF, 10, 0, 166, "WRNLoThCh0"); regMap["WRNLoThCh0"] = &AMACv2Reg::WRNLoThCh0;
  WRNHiThCh1.initReg(m_cfg, RW, 0x3FF, 10, 10, 166, "WRNLoThCh1"); regMap["WRNLoThCh1"] = &AMACv2Reg::WRNLoThCh1;
  WRNHiThCh2.initReg(m_cfg, RW, 0x3FF, 10, 20, 166, "WRNLoThCh2"); regMap["WRNLoThCh2"] = &AMACv2Reg::WRNLoThCh2;
  // 167 - WRNHiTh1
  WRNHiThCh3.initReg(m_cfg, RW, 0x3FF, 10, 0, 167, "WRNHiThCh3"); regMap["WRNHiThCh3"] = &AMACv2Reg::WRNHiThCh3;
  WRNHiThCh4.initReg(m_cfg, RW, 0x3FF, 10, 10, 167, "WRNHiThCh4"); regMap["WRNHiThCh4"] = &AMACv2Reg::WRNHiThCh4;
  WRNHiThCh5.initReg(m_cfg, RW, 0x3FF, 10, 20, 167, "WRNHiThCh5"); regMap["WRNHiThCh5"] = &AMACv2Reg::WRNHiThCh5;
  // 168 - WRNHiTh2
  WRNHiThCh6.initReg(m_cfg, RW, 0x3FF, 10, 0, 168, "WRNHiThCh6"); regMap["WRNHiThCh6"] = &AMACv2Reg::WRNHiThCh6;
  WRNHiThCh7.initReg(m_cfg, RW, 0x3FF, 10, 10, 168, "WRNHiThCh7"); regMap["WRNHiThCh7"] = &AMACv2Reg::WRNHiThCh7;
  WRNHiThCh8.initReg(m_cfg, RW, 0x3FF, 10, 20, 168, "WRNHiThCh8"); regMap["WRNHiThCh8"] = &AMACv2Reg::WRNHiThCh8;
  // 169 - WRNHiTh3
  WRNHiThCh9.initReg(m_cfg, RW, 0x3FF, 10, 0, 169, "WRNHiThCh9"); regMap["WRNHiThCh9"] = &AMACv2Reg::WRNHiThCh9;
  WRNHiThCh10.initReg(m_cfg, RW, 0x3FF, 10, 10, 169, "WRNHiThCh10"); regMap["WRNHiThCh10"] = &AMACv2Reg::WRNHiThCh10;
  WRNHiThCh11.initReg(m_cfg, RW, 0x3FF, 10, 20, 169, "WRNHiThCh11"); regMap["WRNHiThCh11"] = &AMACv2Reg::WRNHiThCh11;
  // 170 - WRNHiTh4
  WRNHiThCh12.initReg(m_cfg, RW, 0x3FF, 10, 0, 170, "WRNHiThCh12"); regMap["WRNHiThCh12"] = &AMACv2Reg::WRNHiThCh12;
  WRNHiThCh13.initReg(m_cfg, RW, 0x3FF, 10, 10, 170, "WRNHiThCh13"); regMap["WRNHiThCh13"] = &AMACv2Reg::WRNHiThCh13;
  WRNHiThCh14.initReg(m_cfg, RW, 0x3FF, 10, 20, 170, "WRNHiThCh14"); regMap["WRNHiThCh14"] = &AMACv2Reg::WRNHiThCh14;
  // 171 - WRNHiTh5
  WRNHiThCh15.initReg(m_cfg, RW, 0x3FF, 10, 0, 171, "WRNHiThCh15"); regMap["WRNHiThCh15"] = &AMACv2Reg::WRNHiThCh15;
}


uint32_t AMACv2Reg::getField(AMACv2Field AMACv2Reg::* ref)
{
  return (this->*ref).read();
}

uint32_t AMACv2Reg::getField(const std::string& fieldName)
{
  if(regMap.find(fieldName) != regMap.end())
    return(this->*regMap[fieldName]).read();
  else
    std::cerr << " --> Error: Could not find register \""<< fieldName << "\"" << std::endl;
  return 0;
}
		
uint32_t AMACv2Reg::getReg(AMACv2Field AMACv2Reg::* ref)
{
  return (this->*ref).readRaw();
}

void AMACv2Reg::setField(AMACv2Field AMACv2Reg::* ref, uint32_t value)
{
  (this->*ref).write(value);
}

void AMACv2Reg::setField(const std::string& fieldName, uint32_t value)
{
  if(regMap.find(fieldName) != regMap.end())
    return(this->*regMap[fieldName]).write(value);
  else
    std::cerr << " --> Error: Could not find register \""<< fieldName << "\"" << std::endl;
  return;
}

void AMACv2Reg::setReg(uint32_t reg, uint32_t value)
{
  m_cfg[reg]=value;
}

void AMACv2Reg::setReg(AMACv2Field AMACv2Reg::* ref, uint32_t value)
{
  (this->*ref).writeRaw(value);
}

uint8_t AMACv2Reg::getAddr(AMACv2Field AMACv2Reg::* ref)
{
  return (this->*ref).addr();
}

uint8_t AMACv2Reg::getAddr(const std::string& fieldName)
{
  if(regMap.find(fieldName) != regMap.end())
    return (this->*regMap[fieldName]).addr();
  else
    std::cerr << " --> Error: Could not find register \""<< fieldName << "\"" << std::endl;
  return 0;
}


bool AMACv2Reg::canBeWritten(AMACv2Field AMACv2Reg::* ref)
{
  return (this->*ref).canBeWrittenField();
}





