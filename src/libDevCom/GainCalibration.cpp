#include "GainCalibration.h"
#include <iostream>
#include <cmath>

GainCalibration::GainCalibration(double reference, uint8_t n_bit, uint8_t gain)
  :m_reference(reference), m_bit(n_bit), m_gain(gain)
{}

GainCalibration::~GainCalibration()
{}

double GainCalibration::calibrate(uint32_t counts)
{
  //Voltage value of the LSB Output
  double val_LSB = 2.0*m_reference/(pow(2.0, (double)m_bit));

  //Gain select
  double gain = 0.0;

  switch(m_gain)
    {
    case 0:
      gain = 1.0;
      break;
    case 1:
      gain = 2.0;
      break;
    case 2:
      gain = 4.0;
      break;
    case 3:
      gain = 8.0;
      break;
    default:
      gain = 1.0;
      break;
    }

  //Voltage value from ADC
  double result;
  
  //Test if the counts number is positve or negative
  if (counts >= (uint32_t) pow(2.0, (double)(m_bit-1)))
    {
      //Conversion in case of negative
      result = -1.0*(pow(2.0, (double)(m_bit))-(double)counts)/gain*val_LSB;
    }else
    {
      //Conversion in case of positive
      result = ((double)counts)/gain*val_LSB;
    }   
  return result; 
}

uint32_t GainCalibration::uncalibrate(double value)
{
  //Voltage value of the LSB Output
  double val_LSB = 2.0*m_reference/(pow(2.0, (double)m_bit));

  //Code value from ADC output
  uint32_t result;

  //Gain select
  double gain = 0.0;

  switch(m_gain)
    {
    case 0:
      gain = 1.0;
      break;
    case 1:
      gain = 2.0;
      break;
    case 2:
      gain = 4.0;
      break;
    case 3:
      gain = 8.0;
      break;
    default:
      gain = 1.0;
      break;
    }

  //Test the sign of voltage Input
  if (value >= 0.0)
    {
      //Conversion in case of postive
      result = (uint32_t)(value*gain/val_LSB);
    }else
    {
      //Store the value in 2's Complement
      int32_t vNeg = (int32_t)(value*gain/val_LSB);
      
      //Generate the compare value
      uint32_t vCompare = 0;
      for(uint8_t i=0; i<m_bit; i++)
	{
	  vCompare += (uint32_t)pow(2.0,(double)i);
	}

      //Take care about the first m_bit bits
      result = vNeg & vCompare;
    }

  return result;
}
