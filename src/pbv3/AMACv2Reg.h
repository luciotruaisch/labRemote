#ifndef AMACREG_H
#define AMACREG_H

#include <memory>
#include <map>
#include <vector>
#include <iostream>
#include <stdint.h>
#include <string>


enum rw_t {RO, WO, RW};

class AMACv2Field
{
private:
  uint32_t *m_cfg = nullptr;
  rw_t m_rw = RO;
  uint32_t m_defaultVal = 0;
  uint8_t m_width = 0;
  uint8_t m_offset = 0;
  uint8_t m_regNbr = 0;
  uint32_t m_mask = 0;
  std::string m_fieldName = "";
public:
  AMACv2Field();

  bool canBeWrittenField() const;
  bool canBeReadField() const;

  std::string getFieldName() const;
		
  void initReg(uint32_t* cfg, rw_t rw, uint32_t defaultVal, uint8_t width, uint8_t offset, uint8_t regNbr, const std::string& fieldName);

  void setDefaultVal(uint32_t defaultVal);
  void writeDefaultVal();

  uint32_t read() const;
  uint32_t readRaw() const;

  void write(const uint32_t& cfgBits);		
  void writeRaw(const uint32_t& cfgBits);

  uint8_t addr() const;
};

class AMACv2Reg
{
private:
  void init();

public:
  static const unsigned numRegs = 171;
  uint32_t m_cfg[numRegs];

  AMACv2Reg();

  std::vector<const AMACv2Field*> getFields() const;

  AMACv2Field* findField(AMACv2Field AMACv2Reg::* ref);
  AMACv2Field* findField(const std::string& fieldName);

  uint32_t getField(AMACv2Field AMACv2Reg::* ref);
  uint32_t getField(const std::string& fieldName);

  uint32_t getReg(uint32_t reg);
  uint32_t getReg(AMACv2Field AMACv2Reg::* ref);
  uint32_t getReg(const std::string& fieldName);

  void setField(AMACv2Field AMACv2Reg::* ref, uint32_t value);
  void setField(const std::string& fieldName, uint32_t value);

  void setReg(uint32_t reg, uint32_t value);
  void setReg(AMACv2Field AMACv2Reg::* ref, uint32_t value);

  uint8_t getAddr(AMACv2Field AMACv2Reg::* ref);
  uint8_t getAddr(const std::string& fieldName);

  bool canBeWritten(AMACv2Field AMACv2Reg::* ref);

  // 0 - Status register
  AMACv2Field StatusAM;
  AMACv2Field StatusWARN;
  AMACv2Field StatusDCDC;
  AMACv2Field StatusHV3;
  AMACv2Field StatusHV2;
  AMACv2Field StatusHV1;
  AMACv2Field StatusHV0;
  AMACv2Field StatusY2LDO;
  AMACv2Field StatusY1LDO;
  AMACv2Field StatusY0LDO;
  AMACv2Field StatusX2LDO;
  AMACv2Field StatusX1LDO;
  AMACv2Field StatusX0LDO;
  AMACv2Field StatusGPI;
  AMACv2Field StatusPGOOD;
  AMACv2Field StatusILockWARN;
  AMACv2Field StatusILockDCDC;
  AMACv2Field StatusILockHV2;
  AMACv2Field StatusILockHV0;
  AMACv2Field StatusILockYLDO;
  AMACv2Field StatusILockxLDO;
  // 1 - HxFlags
  AMACv2Field HxFlagsHi;
  AMACv2Field HxFlagsLo;
  // 2 - HyFlags
  AMACv2Field HyFlagsHi;
  AMACv2Field HyFlagsLo;
  // 3 - HV0Flags
  AMACv2Field HV0FlagsHi;
  AMACv2Field HV0FlagsLo;
  // 4 - HV2Flags
  AMACv2Field HV2FlagsHi;
  AMACv2Field HV2FlagsLo;
  // 5 - DCDCflags
  AMACv2Field DCDCflagsHi;
  AMACv2Field DCDCflagsLo;
  // 6 - WRNflags
  AMACv2Field WRNflagsHi;
  AMACv2Field WRNflagsLo;
  // 7 - SynFlags
  AMACv2Field SynFlagsWRN;
  AMACv2Field SynFlagsDCDC;
  AMACv2Field SynFlagsHV2;
  AMACv2Field SynFlagsHV0;
  AMACv2Field SynFlagsHy;
  AMACv2Field SynFlagsHx;
  // 10 - Value0
  AMACv2Field Value0AMen;
  AMACv2Field Ch0Value;
  AMACv2Field Ch1Value;
  AMACv2Field Ch2Value;
  // 11 - Value1
  AMACv2Field Value1AMen;
  AMACv2Field Ch3Value;
  AMACv2Field Ch4Value;
  AMACv2Field Ch5Value;
  // 12 - Value2
  AMACv2Field Value2AMen;
  AMACv2Field Ch6Value;
  AMACv2Field Ch7Value;
  AMACv2Field Ch8Value;
  // 13 - Value3
  AMACv2Field Value3AMen;
  AMACv2Field Ch9Value;
  AMACv2Field Ch10Value;
  AMACv2Field Ch11Value;
  // 14 - Value4
  AMACv2Field Value4AMen;
  AMACv2Field Ch12Value;
  AMACv2Field Ch13Value;
  AMACv2Field Ch14Value;
  // 15 - Value5
  AMACv2Field Value5AMen;
  AMACv2Field Ch15Value;
  // 31 - SerNum
  AMACv2Field PadID;
  AMACv2Field SerNum;
  // 32 - FlagResets
  AMACv2Field FlagResetWRN;
  AMACv2Field FlagResetDCDC;
  AMACv2Field FlagResetHV2;
  AMACv2Field FlagResetHV0;
  AMACv2Field FlagResetXLDO;
  AMACv2Field FlagResetYLDO;
  // 33 - LogicReset
  AMACv2Field LogicReset;
  // 34 - HardReset
  AMACv2Field HardReset;
  // 40 - CntSet
  AMACv2Field CntSetHV3frq;
  AMACv2Field CntSetHV3en;
  AMACv2Field CntSetHV2frq;
  AMACv2Field CntSetHV2en;
  AMACv2Field CntSetHV1frq;
  AMACv2Field CntSetHV1en;
  AMACv2Field CntSetHV0frq;
  AMACv2Field CntSetHV0en;
  AMACv2Field CntSetHyLDO2en;
  AMACv2Field CntSetHyLDO1en;
  AMACv2Field CntSetHyLDO0en;
  AMACv2Field CntSetHxLDO2en;
  AMACv2Field CntSetHxLDO1en;
  AMACv2Field CntSetHxLDO0en;
  AMACv2Field CntSetWARN;
  // 41 - CntSetC
  AMACv2Field CntSetCHV3frq;
  AMACv2Field CntSetCHV3en;
  AMACv2Field CntSetCHV2frq;
  AMACv2Field CntSetCHV2en;
  AMACv2Field CntSetCHV1frq;
  AMACv2Field CntSetCHV1en;
  AMACv2Field CntSetCHV0frq;
  AMACv2Field CntSetCHV0en;
  AMACv2Field CntSetCHyLDO2en;
  AMACv2Field CntSetCHyLDO1en;
  AMACv2Field CntSetCHyLDO0en;
  AMACv2Field CntSetCHxLDO2en;
  AMACv2Field CntSetCHxLDO1en;
  AMACv2Field CntSetCHxLDO0en;
  AMACv2Field CntSetCWARN;
  // 42 - DCDCen
  AMACv2Field DCDCAdj;
  AMACv2Field DCDCen;
  // 43 - DCDCenC
  AMACv2Field DCDCAdjC;
  AMACv2Field DCDCenC;
  // 44 - Ilock
  AMACv2Field IlockWRN;
  AMACv2Field IlockDCDC;
  AMACv2Field IlockHV2;
  AMACv2Field IlockHV0;
  AMACv2Field IlockHy;
  AMACv2Field IlockHx;
  // 45 - IlockC
  AMACv2Field IlockCWRN;
  AMACv2Field IlockCDCDC;
  AMACv2Field IlockCHV2;
  AMACv2Field IlockCHV0;
  AMACv2Field IlockCHy;
  AMACv2Field IlockCHx;
  // 46 - RstCnt
  AMACv2Field RstCntHyHCCresetB;
  AMACv2Field RstCntHxHCCresetB;
  AMACv2Field RstCntOF;
  // 47 - RstCntC
  AMACv2Field RstCntCHyHCCresetB;
  AMACv2Field RstCntCHxHCCresetB;
  AMACv2Field RstCntCOF;
  // 48 - AMen
  AMACv2Field AMzeroCalib;
  AMACv2Field AMen;
  // 49 - AMenC
  AMACv2Field AMzeroCalibC;
  AMACv2Field AMenC;
  // 50 - AMpwr
  AMACv2Field ReqDCDCPGOOD;
  AMACv2Field DCDCenToPwrAMAC;
  // 51 - AMpwrC
  AMACv2Field ReqDCDCPGOODC;
  AMACv2Field DCDCenToPwrAMACC;
  // 52 - BgCnt
  AMACv2Field AMbgen;
  AMACv2Field AMbg;
  AMACv2Field VDDbgen;
  AMACv2Field VDDbg;
  // 53 - AMcnt
  AMACv2Field AMintCalib;
  AMACv2Field Ch13Mux;
  AMACv2Field Ch12Mux;
  AMACv2Field Ch5Mux;
  AMACv2Field Ch4Mux;
  AMACv2Field Ch3Mux;
  // 54 - Dacs0
  AMACv2Field DACShunty;
  AMACv2Field DACShuntx;
  AMACv2Field DACCALy;
  AMACv2Field DACCalx;
  // 55 - DACbias
  AMACv2Field DACbias;
  // 56 - AMACcnt
  AMACv2Field HVcurGain;
  AMACv2Field DRcomMode;
  AMACv2Field DRcurr;
  AMACv2Field RingOscFrq;
  // 57 - NTC
  AMACv2Field NTCpbCal;
  AMACv2Field NTCpbSenseRange;
  AMACv2Field NTCy0Cal;
  AMACv2Field NTCy0SenseRange;
  AMACv2Field NTCx0Cal;
  AMACv2Field NTCx0SenseRange;
  // 58 - LvCurCal
  AMACv2Field DCDCoOffset;
  AMACv2Field DCDCoZeroReading;
  AMACv2Field DCDCoP;
  AMACv2Field DCDCoN;
  AMACv2Field DCDCiZeroReading;
  AMACv2Field DCDCiRangeSW;
  AMACv2Field DCDCiOffset;
  AMACv2Field DCDCiP;
  AMACv2Field DCDCiN;
  // 60 - HxICcfg
  AMACv2Field HxLAM;
  AMACv2Field HxFlagsLatch;
  AMACv2Field HxFlagsLogic;
  AMACv2Field HxFlagValid;
  AMACv2Field HxFlagsValidConf;
  // 61 - HyICcfg
  AMACv2Field HyLAM;
  AMACv2Field HyFlagsLatch;
  AMACv2Field HyFlagsLogic;
  AMACv2Field HyFlagValid;
  AMACv2Field HyFlagsValidConf;
  // 62 - HV0ICcfg
  AMACv2Field HV0LAM;
  AMACv2Field HV0FlagsLatch;
  AMACv2Field HV0FlagsLogic;
  AMACv2Field HV0FlagValid;
  AMACv2Field HV0FlagsValidConf;
  // 63 - HV2ICcfg
  AMACv2Field HV2LAM;
  AMACv2Field HV2FlagsLatch;
  AMACv2Field HV2FlagsLogic;
  AMACv2Field HV2FlagValid;
  AMACv2Field HV2FlagsValidConf;
  // 64 - DCDCICcfg
  AMACv2Field DCDCLAM;
  AMACv2Field DCDCFlagsLatch;
  AMACv2Field DCDCFlagsLogic;
  AMACv2Field DCDCFlagValid;
  AMACv2Field DCDCFlagsValidConf;
  // 65 - WRNICcfg
  AMACv2Field WRNLAM;
  AMACv2Field WRNFlagsLatch;
  AMACv2Field WRNFlagsLogic;
  AMACv2Field WRNFlagValid;
  AMACv2Field WRNFlagsValidConf;
  // 70 - HxTlut
  AMACv2Field HxTlut;
  // 71 - HxModlut1
  AMACv2Field HxModlut1;
  // 72 - HxModlut2
  AMACv2Field HxModlut2;
  // 73 - HyTlut
  AMACv2Field HyTlut;
  // 74 - HyModlut1
  AMACv2Field HyModlut1;
  // 75 - HyModlut2
  AMACv2Field HyModlut2;
  // 76 - HV0Tlut;
  AMACv2Field HV0Tlut;
  // 77 - HV0Modlut1
  AMACv2Field HV0Modlut1;
  // 78 - HV0Modlut2
  AMACv2Field HV0Modlut2;
  // 79 - HV2Tlut
  AMACv2Field HV2Tlut;
  // 80 - HV2Modlut1
  AMACv2Field HV2Modlut1;
  // 81 - HV2Modlut2
  AMACv2Field HV2Modlut2;
  // 82 - DCDCTlut
  AMACv2Field DCDCTlut;
  // 83 - DCDCModlut1
  AMACv2Field DCDCModlut1;
  // 84 - DCDCModlut2
  AMACv2Field DCDCModlut2	;
  // 85 - WRNTlut
  AMACv2Field WRNTlut;
  // 86 - WRNModlut1
  AMACv2Field WRNModlut1;
  // 87 - WRNModlut2
  AMACv2Field WRNModlut2;
  // 90 - HxFlagEn
  AMACv2Field HxFlagsEnHi;
  AMACv2Field HxFlagsEnLo;
  // 91 - HyFlagEn
  AMACv2Field HyFlagsEnHi;
  AMACv2Field HyFlagsEnLo;
  // 92 - HV0FlagEn
  AMACv2Field HV0FlagsEnHi;
  AMACv2Field HV0FlagsEnLo;
  // 93 - HV2FlagEn
  AMACv2Field HV2FlagsEnHi;
  AMACv2Field HV2FlagsEnLo;
  // 94 - DCDCFlagEn
  AMACv2Field DCDCFlagsEnHi;
  AMACv2Field DCDCFlagsEnLo;
  // 94 - WRNFlagEn
  AMACv2Field WRNFlagsEnHi;
  AMACv2Field WRNFlagsEnLo;
  // 95 - SynFlagEn
  AMACv2Field WRNsynFlagEnHi;
  AMACv2Field WRNsynFlagEnLo;
  AMACv2Field DCDCsynFlagEnHi;
  AMACv2Field DCDCsynFlagEnLo;
  AMACv2Field HV2synFlagEnHi;
  AMACv2Field HV2synFlagEnLo;
  AMACv2Field HV0synFlagEnHi;
  AMACv2Field HV0synFlagEnLo;
  AMACv2Field HysynFlagEnHi;
  AMACv2Field HysynFlagEnLo;
  AMACv2Field HxsynFlagEnHi;
  AMACv2Field HxsynFlagEnLo;
  // 100 - HxLoTh0
  AMACv2Field HxLoThCh0;
  AMACv2Field HxLoThCh1;
  AMACv2Field HxLoThCh2;
  // 101 - HxLoTh1
  AMACv2Field HxLoThCh3;
  AMACv2Field HxLoThCh4;
  AMACv2Field HxLoThCh5;
  // 102 - HxLoTh2
  AMACv2Field HxLoThCh6;
  AMACv2Field HxLoThCh7;
  AMACv2Field HxLoThCh8;
  // 103 - HxLoTh3
  AMACv2Field HxLoThCh9;
  AMACv2Field HxLoThCh10;
  AMACv2Field HxLoThCh11;
  // 104 - HxLoTh4
  AMACv2Field HxLoThCh12;
  AMACv2Field HxLoThCh13;
  AMACv2Field HxLoThCh14;
  // 105 - HxLoTh5
  AMACv2Field HxLoThCh15;
  // 106 - HxHiTh0
  AMACv2Field HxHiThCh0;
  AMACv2Field HxHiThCh1;
  AMACv2Field HxHiThCh2;
  // 107 - HxHiTh1
  AMACv2Field HxHiThCh3;
  AMACv2Field HxHiThCh4;
  AMACv2Field HxHiThCh5;
  // 108 - HxHiTh2
  AMACv2Field HxHiThCh6;
  AMACv2Field HxHiThCh7;
  AMACv2Field HxHiThCh8;
  // 109 - HxHiTh3
  AMACv2Field HxHiThCh9;
  AMACv2Field HxHiThCh10;
  AMACv2Field HxHiThCh11;
  // 110 - HxHiTh4
  AMACv2Field HxHiThCh12;
  AMACv2Field HxHiThCh13;
  AMACv2Field HxHiThCh14;
  // 111 - HxHiTh5
  AMACv2Field HxHiThCh15;
  // 112 - HyLoTh0
  AMACv2Field HyLoThCh0;
  AMACv2Field HyLoThCh1;
  AMACv2Field HyLoThCh2;
  // 113 - HyLoTh1
  AMACv2Field HyLoThCh3;
  AMACv2Field HyLoThCh4;
  AMACv2Field HyLoThCh5;
  // 114 - HyLoTh2
  AMACv2Field HyLoThCh6;
  AMACv2Field HyLoThCh7;
  AMACv2Field HyLoThCh8;
  // 115 - HyLoTh3
  AMACv2Field HyLoThCh9;
  AMACv2Field HyLoThCh10;
  AMACv2Field HyLoThCh11;
  // 116 - HyLoTh4
  AMACv2Field HyLoThCh12;
  AMACv2Field HyLoThCh13;
  AMACv2Field HyLoThCh14;
  // 117 - HyLoTh5
  AMACv2Field HyLoThCh15;
  // 118 - HyHiTh0
  AMACv2Field HyHiThCh0;
  AMACv2Field HyHiThCh1;
  AMACv2Field HyHiThCh2;
  // 119 - HyHiTh1
  AMACv2Field HyHiThCh3;
  AMACv2Field HyHiThCh4;
  AMACv2Field HyHiThCh5;
  // 120 - HyHiTh2
  AMACv2Field HyHiThCh6;
  AMACv2Field HyHiThCh7;
  AMACv2Field HyHiThCh8;
  // 121 - HyHiTh3
  AMACv2Field HyHiThCh9;
  AMACv2Field HyHiThCh10;
  AMACv2Field HyHiThCh11;
  // 122 - HyHiTh4
  AMACv2Field HyHiThCh12;
  AMACv2Field HyHiThCh13;
  AMACv2Field HyHiThCh14;
  // 123 - HyHiTh5
  AMACv2Field HyHiThCh15;
  // 124 - HV0LoTh0
  AMACv2Field HV0LoThCh0;
  AMACv2Field HV0LoThCh1;
  AMACv2Field HV0LoThCh2;
  // 125 - HV0LoTh1
  AMACv2Field HV0LoThCh3;
  AMACv2Field HV0LoThCh4;
  AMACv2Field HV0LoThCh5;
  // 126 - HV0LoTh2
  AMACv2Field HV0LoThCh6;
  AMACv2Field HV0LoThCh7;
  AMACv2Field HV0LoThCh8;
  // 127 - HV0LoTh3
  AMACv2Field HV0LoThCh9;
  AMACv2Field HV0LoThCh10;
  AMACv2Field HV0LoThCh11;
  // 128 - HV0LoTh4
  AMACv2Field HV0LoThCh12;
  AMACv2Field HV0LoThCh13;
  AMACv2Field HV0LoThCh14;
  // 129 - HV0LoTh5
  AMACv2Field HV0LoThCh15;
  // 130 - HV0HiTh0
  AMACv2Field HV0HiThCh0;
  AMACv2Field HV0HiThCh1;
  AMACv2Field HV0HiThCh2;
  // 131 - HV0HiTh1
  AMACv2Field HV0HiThCh3;
  AMACv2Field HV0HiThCh4;
  AMACv2Field HV0HiThCh5;
  // 132 - HV0HiTh2
  AMACv2Field HV0HiThCh6;
  AMACv2Field HV0HiThCh7;
  AMACv2Field HV0HiThCh8;
  // 133 - HV0HiTh3
  AMACv2Field HV0HiThCh9;
  AMACv2Field HV0HiThCh10;
  AMACv2Field HV0HiThCh11;
  // 134 - HV0HiTh4
  AMACv2Field HV0HiThCh12;
  AMACv2Field HV0HiThCh13;
  AMACv2Field HV0HiThCh14;
  // 135 - HV0HiTh5
  AMACv2Field HV0HiThCh15;
  // 136 - HV2LoTh0
  AMACv2Field HV2LoThCh0;
  AMACv2Field HV2LoThCh1;
  AMACv2Field HV2LoThCh2;
  // 137 - HV2LoTh1
  AMACv2Field HV2LoThCh3;
  AMACv2Field HV2LoThCh4;
  AMACv2Field HV2LoThCh5;
  // 138 - HV2LoTh2
  AMACv2Field HV2LoThCh6;
  AMACv2Field HV2LoThCh7;
  AMACv2Field HV2LoThCh8;
  // 139 - HV2LoTh3
  AMACv2Field HV2LoThCh9;
  AMACv2Field HV2LoThCh10;
  AMACv2Field HV2LoThCh11;
  // 140 - HV2LoTh4
  AMACv2Field HV2LoThCh12;
  AMACv2Field HV2LoThCh13;
  AMACv2Field HV2LoThCh14;
  // 141 - HV2LoTh5
  AMACv2Field HV2LoThCh15;
  // 142 - HV2HiTh0
  AMACv2Field HV2HiThCh0;
  AMACv2Field HV2HiThCh1;
  AMACv2Field HV2HiThCh2;
  // 143 - HV2HiTh1
  AMACv2Field HV2HiThCh3;
  AMACv2Field HV2HiThCh4;
  AMACv2Field HV2HiThCh5;
  // 144 - HV2HiTh2
  AMACv2Field HV2HiThCh6;
  AMACv2Field HV2HiThCh7;
  AMACv2Field HV2HiThCh8;
  // 145 - HV2HiTh3
  AMACv2Field HV2HiThCh9;
  AMACv2Field HV2HiThCh10;
  AMACv2Field HV2HiThCh11;
  // 146 - HV2HiTh4
  AMACv2Field HV2HiThCh12;
  AMACv2Field HV2HiThCh13;
  AMACv2Field HV2HiThCh14;
  // 147 - HV2HiTh5
  AMACv2Field HV2HiThCh15;
  // 148 - DCDCLoTh0
  AMACv2Field DCDCLoThCh0;
  AMACv2Field DCDCLoThCh1;
  AMACv2Field DCDCLoThCh2;
  // 149 - DCDCLoTh1
  AMACv2Field DCDCLoThCh3;
  AMACv2Field DCDCLoThCh4;
  AMACv2Field DCDCLoThCh5;
  // 150 - DCDCLoTh2
  AMACv2Field DCDCLoThCh6;
  AMACv2Field DCDCLoThCh7;
  AMACv2Field DCDCLoThCh8;
  // 151 - DCDCLoTh3
  AMACv2Field DCDCLoThCh9;
  AMACv2Field DCDCLoThCh10;
  AMACv2Field DCDCLoThCh11;
  // 152 - DCDCLoTh4
  AMACv2Field DCDCLoThCh12;
  AMACv2Field DCDCLoThCh13;
  AMACv2Field DCDCLoThCh14;
  // 153 - DCDCLoTh5
  AMACv2Field DCDCLoThCh15;
  // 154 - DCDCHiTh0
  AMACv2Field DCDCHiThCh0;
  AMACv2Field DCDCHiThCh1;
  AMACv2Field DCDCHiThCh2;
  // 155 - DCDCHiTh1
  AMACv2Field DCDCHiThCh3;
  AMACv2Field DCDCHiThCh4;
  AMACv2Field DCDCHiThCh5;
  // 156 - DCDCHiTh2
  AMACv2Field DCDCHiThCh6;
  AMACv2Field DCDCHiThCh7;
  AMACv2Field DCDCHiThCh8;
  // 157 - DCDCHiTh3
  AMACv2Field DCDCHiThCh9;
  AMACv2Field DCDCHiThCh10;
  AMACv2Field DCDCHiThCh11;
  // 158 - DCDCHiTh4
  AMACv2Field DCDCHiThCh12;
  AMACv2Field DCDCHiThCh13;
  AMACv2Field DCDCHiThCh14;
  // 159 - DCDCHiTh5
  AMACv2Field DCDCHiThCh15;
  // 160 - WRNLoTh0
  AMACv2Field WRNLoThCh0;
  AMACv2Field WRNLoThCh1;
  AMACv2Field WRNLoThCh2;
  // 161 - WRNLoTh1
  AMACv2Field WRNLoThCh3;
  AMACv2Field WRNLoThCh4;
  AMACv2Field WRNLoThCh5;
  // 162 - WRNLoTh2
  AMACv2Field WRNLoThCh6;
  AMACv2Field WRNLoThCh7;
  AMACv2Field WRNLoThCh8;
  // 163 - WRNLoTh3
  AMACv2Field WRNLoThCh9;
  AMACv2Field WRNLoThCh10;
  AMACv2Field WRNLoThCh11;
  // 164 - WRNLoTh4
  AMACv2Field WRNLoThCh12;
  AMACv2Field WRNLoThCh13;
  AMACv2Field WRNLoThCh14;
  // 165 - WRNLoTh5
  AMACv2Field WRNLoThCh15;
  // 166 - WRNHiTh0;
  AMACv2Field WRNHiThCh0;
  AMACv2Field WRNHiThCh1;
  AMACv2Field WRNHiThCh2;
  // 167 - WRNHiTh1
  AMACv2Field WRNHiThCh3;
  AMACv2Field WRNHiThCh4;
  AMACv2Field WRNHiThCh5;
  // 168 - WRNHiTh2
  AMACv2Field WRNHiThCh6;
  AMACv2Field WRNHiThCh7;
  AMACv2Field WRNHiThCh8;
  // 169 - WRNHiTh3
  AMACv2Field WRNHiThCh9;
  AMACv2Field WRNHiThCh10;
  AMACv2Field WRNHiThCh11;
  // 170 - WRNHiTh4
  AMACv2Field WRNHiThCh12;
  AMACv2Field WRNHiThCh13;
  AMACv2Field WRNHiThCh14;
  // 171 - WRNHiTh5
  AMACv2Field WRNHiThCh15;

protected:
  std::map<std::string, AMACv2Field AMACv2Reg::*> regMap;
};
#endif //AMACREG_H
