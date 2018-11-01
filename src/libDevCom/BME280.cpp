#include "BME280.h"

#include <iostream>
#include <iomanip>

BME280::BME280(std::shared_ptr<SPICom> devcom)
  : m_devcom(devcom)
{ }

BME280::~BME280()
{ }

void BME280::init()
{
  load_calibration();
}

void BME280::reset()
{
  m_devcom->write_reg(0x60, 0xB6);
}

float BME280::temperature()
{
  uint8_t data[3];
  m_devcom->read_reg(0xFA,data,3);
  int adc_T=(data[0]<<16)|(data[1]<<8)|(data[2]);
  adc_T>>=4;

  int var1, var2, T;
  var1 = (( ((adc_T>>3) - (m_dig_T1<<1)) ) * (m_dig_T2)) >> 11;
  var2 = (((((adc_T>>4) - (m_dig_T1)) * ((adc_T>>4) - (m_dig_T1))) >> 12) * (m_dig_T3)) >> 14;
  m_t_fine = var1 + var2;
  T = (m_t_fine * 5 + 128) >> 8;

  return T/100.;
}

float BME280::humidity()
{
  temperature(); // must be done first to get t_fine

  uint8_t data[2];
  m_devcom->read_reg(0xFD,data,2);
  int adc_H=(data[0]<<8)|(data[1]);

  int32_t v_x1_u32r;

  v_x1_u32r = (m_t_fine - ((int32_t)76800));

  v_x1_u32r = (((((adc_H << 14) - (((int32_t)m_dig_H4) << 20) -
   		  (((int32_t)m_dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
	       (((((((v_x1_u32r * ((int32_t)m_dig_H6)) >> 10) *
		    (((v_x1_u32r * ((int32_t)m_dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
		  ((int32_t)2097152)) * ((int32_t)m_dig_H2) + 8192) >> 14));

  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
   			     ((int32_t)m_dig_H1)) >> 4));

  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
  float h = (v_x1_u32r>>12);
  return  h / 1024.0;
}

float BME280::pressure()
{
  temperature(); // must be done first to get t_fine

  uint8_t data[3];
  m_devcom->read_reg(0xF7,data,3);

  int adc_P=(data[0]<<16)|(data[1]<<8)|(data[2]);
  adc_P>>=4;

  int64_t var1, var2, p;
  var1 = ((int64_t)m_t_fine) - 128000;
  var2 = var1 * var1 * (int64_t)m_dig_P6;
  var2 = var2 + ((var1*(int64_t)m_dig_P5)<<17);
  var2 = var2 + (((int64_t)m_dig_P4)<<35);
  var1 = ((var1 * var1 * (int64_t)m_dig_P3)>>8) +
    ((var1 * (int64_t)m_dig_P2)<<12);
  var1 = (((((int64_t)1)<<47)+var1))*((int64_t)m_dig_P1)>>33;

  if (var1 == 0)
    return 0; // avoid exception caused by division by zero

  p = 1048576 - adc_P;
  p = (((p<<31) - var2)*3125) / var1;
  var1 = (((int64_t)m_dig_P9) * (p>>13) * (p>>13)) >> 25;
  var2 = (((int64_t)m_dig_P8) * p) >> 19;

  p = ((p + var1 + var2) >> 8) + (((int64_t)m_dig_P7)<<4);
  return (float)p/256;
}

void BME280::load_calibration()
{
  //
  // Calibrate temperature
  uint8_t calibTdata[6];
  m_devcom->read_reg(0x88,calibTdata,6);
  m_dig_T1=(calibTdata[1]<<8)|calibTdata[0];
  m_dig_T2=(calibTdata[3]<<8)|calibTdata[2];
  m_dig_T3=(calibTdata[5]<<8)|calibTdata[4];

  //
  // Calibrate pressure
  uint8_t calibPdata[16];
  m_devcom->read_reg(0x90,calibPdata,16);
  m_dig_P1=(calibPdata[ 1]<<8)|calibPdata[ 0];
  m_dig_P2=(calibPdata[ 3]<<8)|calibPdata[ 2];
  m_dig_P3=(calibPdata[ 5]<<8)|calibPdata[ 4];
  m_dig_P4=(calibPdata[ 7]<<8)|calibPdata[ 6];
  m_dig_P5=(calibPdata[ 9]<<8)|calibPdata[ 8];
  m_dig_P6=(calibPdata[11]<<8)|calibPdata[10];
  m_dig_P7=(calibPdata[13]<<8)|calibPdata[12];
  m_dig_P8=(calibPdata[15]<<8)|calibPdata[14];
  m_dig_P9=(calibPdata[17]<<8)|calibPdata[16];

  //
  // Calibrate humidity
  m_dig_H1=m_devcom->read_reg(0xA1);

  uint8_t calibHdata[7];
  m_devcom->read_reg(0xE1,calibHdata,7);
  m_dig_H2=(calibHdata[ 1]<<8)|calibHdata[ 0];
  m_dig_H3= calibHdata[ 2];
  m_dig_H4=(calibHdata[ 3]<<4)|(calibHdata[ 4]&0xF);
  m_dig_H5=(calibHdata[ 5]<<4)|(calibHdata[ 4]>>4 );
  m_dig_H6= calibHdata[ 6];
}

void BME280::dump()
{
  //
  // Print device ID
  std::cout << "device id = 0x" << std::hex << m_devcom->read_reg(0xD0) << std::dec << std::endl;

  //
  // Read the calibration
  std::cout << std::endl << "calibration" << std::endl;
  std::cout << "dig_T1 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_T1 << std::dec << std::endl;
  std::cout << "dig_T2 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_T2 << std::dec << std::endl;
  std::cout << "dig_T3 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_T3 << std::dec << std::endl;

  std::cout << "dig_P1 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P1 << std::dec << std::endl;
  std::cout << "dig_P2 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P2 << std::dec << std::endl;
  std::cout << "dig_P3 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P3 << std::dec << std::endl;
  std::cout << "dig_P4 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P4 << std::dec << std::endl;
  std::cout << "dig_P5 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P5 << std::dec << std::endl;
  std::cout << "dig_P6 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P6 << std::dec << std::endl;
  std::cout << "dig_P7 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P7 << std::dec << std::endl;
  std::cout << "dig_P8 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P8 << std::dec << std::endl;
  std::cout << "dig_P9 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_P9 << std::dec << std::endl;

  std::cout << "dig_H1 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_H1 << std::dec << std::endl;
  std::cout << "dig_H2 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_H2 << std::dec << std::endl;
  std::cout << "dig_H3 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_H3 << std::dec << std::endl;
  std::cout << "dig_H4 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_H4 << std::dec << std::endl;
  std::cout << "dig_H5 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_H5 << std::dec << std::endl;
  std::cout << "dig_H6 = 0x" << std::hex << std::setw(4) << std::setfill('0') << m_dig_H6 << std::dec << std::endl;


  //
  // Dump all registers
  std::cout << std::endl << "all registers" << std::endl;
  std::cout << std::hex;
  for(uint i=0;i<8;i++)
    {
      uint8_t reg=0x80+(i<<4);
      std::cout << "0x" << std::setw(2) << std::setfill('0') << (uint)reg << ":";

      uint8_t regdata[16];
      m_devcom->read_reg(reg,regdata,16);
      for(uint j=0;j<16;j++)
	std::cout << " " << std::setw(2) << std::setfill('0') << (uint)regdata[j];
      
      std::cout << std::endl;
    }
  std::cout << std::dec;
}
